#include "zlib_adapter.h"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <zlib.h>

namespace zlib_adapter
{
    constexpr int ZBUF_SIZE = 4096;

    struct ZLibFile::inflater_impl
    {
        FILE*       m_in;
        z_stream    m_zstream{};
        long        m_initial_stream_pos;
        int         m_logical_stream_pos;
        bool        m_at_eof;
        unsigned char m_rawdata[ZBUF_SIZE]{};
        int         m_error;

        explicit inflater_impl(FILE* in)
            : m_in(in)
            , m_initial_stream_pos(ftell(in))
            , m_logical_stream_pos(0)
            , m_at_eof(false)
            , m_error(0)
        {
            assert(m_in);

            m_zstream.zalloc = nullptr;
            m_zstream.zfree  = nullptr;
            m_zstream.opaque = nullptr;

            int err = inflateInit(&m_zstream);
            if (err != Z_OK) {
                m_error = 1;
            }
        }

        ~inflater_impl()
        {
            if (!m_error) {
                inflateEnd(&m_zstream);
            }
        }

        void reset()
        {
            m_error = 0;
            m_at_eof = false;
            int err = inflateReset(&m_zstream);
            if (err != Z_OK) {
                m_error = 1;
                return;
            }

            m_zstream.next_in = nullptr;
            m_zstream.avail_in = 0;
            m_zstream.next_out = nullptr;
            m_zstream.avail_out = 0;

            fseek(m_in, m_initial_stream_pos, SEEK_SET);
            m_logical_stream_pos = 0;
        }

        int inflate_from_stream(void* dst, int bytes)
        {
            if (m_error) return 0;

            m_zstream.next_out = static_cast<unsigned char*>(dst);
            m_zstream.avail_out = bytes;

            while (m_zstream.avail_out > 0)
            {
                if (m_zstream.avail_in == 0)
                {
                    size_t new_bytes = fread(m_rawdata, 1, ZBUF_SIZE, m_in);
                    if (new_bytes == 0) {
                        break;
                    }
                    m_zstream.next_in = m_rawdata;
                    m_zstream.avail_in = static_cast<uInt>(new_bytes);
                }

                int err = inflate(&m_zstream, Z_SYNC_FLUSH);
                if (err == Z_STREAM_END) {
                    m_at_eof = true;
                    break;
                }
                if (err != Z_OK) {
                    m_error = 1;
                    break;
                }
            }

            int bytes_read = bytes - static_cast<int>(m_zstream.avail_out);
            m_logical_stream_pos += bytes_read;
            return bytes_read;
        }

        void rewind_unused_bytes()
        {
            if (m_zstream.avail_in > 0)
            {
                long pos = ftell(m_in);
                long rewound_pos = pos - m_zstream.avail_in;
                assert(pos >= 0 && rewound_pos >= 0);
                fseek(m_in, rewound_pos, SEEK_SET);
            }
        }
    };

ZLibFile::ZLibFile(FILE* in)
    : m_impl(std::make_unique<inflater_impl>(in))
{
}

ZLibFile::~ZLibFile() = default;

int ZLibFile::read(void* dst, int bytes)
{
    if (m_impl->m_error) return 0;
    return m_impl->inflate_from_stream(dst, bytes);
}

int ZLibFile::seek(int pos)
{
    if (m_impl->m_error) return m_impl->m_logical_stream_pos;

    if (pos < m_impl->m_logical_stream_pos) {
        m_impl->reset();
    }

    unsigned char temp[ZBUF_SIZE];
    while (m_impl->m_logical_stream_pos < pos)
    {
        int to_read = pos - m_impl->m_logical_stream_pos;
        int to_read_this_time = std::min(to_read, ZBUF_SIZE);
        int bytes_read = m_impl->inflate_from_stream(temp, to_read_this_time);
        if (bytes_read == 0) break;
    }

    return m_impl->m_logical_stream_pos;
}

int ZLibFile::seek_to_end()
{
    if (m_impl->m_error) return m_impl->m_logical_stream_pos;

    unsigned char temp[ZBUF_SIZE];
    while (true)
    {
        int bytes_read = m_impl->inflate_from_stream(temp, ZBUF_SIZE);
        if (bytes_read == 0) break;
    }
    return m_impl->m_logical_stream_pos;
}

int ZLibFile::tell()
{
    return m_impl->m_logical_stream_pos;
}

bool ZLibFile::get_eof()
{
    return m_impl->m_at_eof;
}

int ZLibFile::close()
{
    m_impl->rewind_unused_bytes();
    int err = inflateEnd(&(m_impl->m_zstream));
    return (err == Z_OK) ? 0 : -1;
}

int DecompressMemory(void* data_in, int size_in, std::vector<unsigned char>& data_out)
{
    z_stream zstr{};
    if (inflateInit(&zstr) != Z_OK) return 0;

    zstr.next_in   = static_cast<unsigned char*>(data_in);
    zstr.avail_in  = static_cast<uInt>(size_in);
    zstr.next_out  = data_out.data();
    zstr.avail_out = static_cast<uInt>(data_out.size());

    while (zstr.avail_out > 0)
    {
        int err = inflate(&zstr, Z_SYNC_FLUSH);
        if (err == Z_STREAM_END) break;
        if (err != Z_OK) {
            inflateEnd(&zstr);
            return 0;
        }
    }
    inflateEnd(&zstr);
    return static_cast<int>(data_out.size() - zstr.avail_out);
}

int CompressMemory(void* data_in, int size_in, std::vector<unsigned char>& data_out)
{
    z_stream zstr{};
    if (deflateInit(&zstr, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    zstr.next_in  = static_cast<unsigned char*>(data_in);
    zstr.avail_in = static_cast<uInt>(size_in);

    std::vector<unsigned char> tempBuffer(size_in + (size_in / 100) + 12);
    zstr.next_out  = tempBuffer.data();
    zstr.avail_out = static_cast<uInt>(tempBuffer.size());

    int err = deflate(&zstr, Z_FINISH);
    deflateEnd(&zstr);
    if (err < 0) return 0;

    data_out.resize(zstr.total_out);
    std::memcpy(data_out.data(), tempBuffer.data(), zstr.total_out);
    return static_cast<int>(data_out.size());
}

} // namespace zlib_adapter