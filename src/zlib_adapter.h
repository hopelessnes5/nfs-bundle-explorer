#ifndef ZLIB_ADAPTER_H
#define ZLIB_ADAPTER_H

#include <vector>
#include <cstdio>
#include <memory>

namespace zlib_adapter
{

class ZLibFile
{
public:
    explicit ZLibFile(FILE* in);
    ~ZLibFile();

    // Запрещаем случайное копирование потока
    ZLibFile(const ZLibFile&) = delete;
    ZLibFile& operator=(const ZLibFile&) = delete;

    int     read(void* dst, int bytes);
    int     seek(int pos);
    int     seek_to_end();
    int     tell();
    bool    get_eof();
    int     close();

private:
    struct inflater_impl;
    std::unique_ptr<inflater_impl> m_impl;
};

int DecompressMemory(void* data_in, int size_in, std::vector<unsigned char>& data_out);
int CompressMemory(void* data_in, int size_in, std::vector<unsigned char>& data_out);

}

#endif // ZLIB_ADAPTER_H