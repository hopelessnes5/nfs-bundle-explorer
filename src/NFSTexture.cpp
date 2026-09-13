#include "NFSTexture.h"
#include <QFile>
#include <cstring>
#include <algorithm>
#include <vector>

static int GetImageSizeForType(int type, int w, int h, int numMips)
{
    int size = 0;
    while (numMips > 0)
    {
        if (type == FOURCC_DXT5)
            size += (((w + 3) >> 2) * ((h + 3) >> 2)) << 4;
        else if (type == FOURCC_DXT1)
            size += (((w + 3) >> 2) * ((h + 3) >> 2)) << 3;
        else
            size += w * h * 4;

        if (w == 1 && h == 1)
            break;
        if (w > 1)
            w >>= 1;
        if (h > 1)
            h >>= 1;

        numMips--;
    }

    return size;
}

bool CNFSTexture::IsNFSTextureSignature(const Uint8Vector& headerData)
{
    if (headerData.size() >= sizeof(S_NFSTextureHeader))
    {
        const auto* hdr = reinterpret_cast<const S_NFSTextureHeader*>(headerData.data());
        if (hdr->type == FOURCC_DXT1 || hdr->type == FOURCC_DXT5 || hdr->type == 21)
            return true;
    }

    return false;
}

bool CNFSTexture::ConvertDDS2NFS(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    DDS_HEADER ddsHdr{};
    uint32 magic = 0;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    file.read(reinterpret_cast<char*>(&ddsHdr), sizeof(ddsHdr));

    S_NFSTextureHeader nfsHdr{};

    if (ddsHdr.ddspf.dwFlags & DDS_FOURCC)
    {
        if (ddsHdr.ddspf.dwFourCC != FOURCC_DXT1 && ddsHdr.ddspf.dwFourCC != FOURCC_DXT5)
            return false;
        nfsHdr.type = ddsHdr.ddspf.dwFourCC;
    }
    else if (ddsHdr.ddspf.dwFlags & DDS_RGBA)
    {
        if (ddsHdr.ddspf.dwRGBBitCount != 32)
            return false;
        nfsHdr.type = 21;
    }

    nfsHdr.width = static_cast<uint16>(ddsHdr.dwWidth);
    nfsHdr.height = static_cast<uint16>(ddsHdr.dwHeight);

    nfsHdr.stuff2 = nfsHdr.stuff3 = 1;
    nfsHdr.stuff4 = 0x3000;
    nfsHdr.unknown = 1;
    nfsHdr.numMipmaps = static_cast<uint8>(std::max(1, static_cast<int>(ddsHdr.dwMipMapCount)));

    int imageDataSize = GetImageSizeForType(nfsHdr.type, nfsHdr.width, nfsHdr.height, nfsHdr.numMipmaps);

    QString newName = fileName;
    newName = newName.replace(".dds", ".nfs", Qt::CaseInsensitive);
    QFile outFile(newName);
    if (!outFile.open(QIODevice::WriteOnly))
        return false;

    constexpr uint32 nfsoMagic = MAKEFOURCC('N', 'F', 'S', 'O');
    constexpr uint32 fileType  = NFS_FILE_TEXTURE;
    outFile.write(reinterpret_cast<const char*>(&nfsoMagic), sizeof(nfsoMagic));
    outFile.write(reinterpret_cast<const char*>(&fileType), sizeof(fileType));
    outFile.write(reinterpret_cast<const char*>(&nfsHdr), sizeof(nfsHdr));

    // Безопасный буфер без утечек памяти
    std::vector<char> imageData(imageDataSize);
    file.read(imageData.data(), imageDataSize);
    outFile.write(imageData.data(), imageDataSize);

    return true;
}

CNFSTexture::CNFSTexture() = default;

int CNFSTexture::InitFromData(const Uint8Vector& headerData, const Uint8Vector& imageData)
{
    if (headerData.size() < sizeof(S_NFSTextureHeader))
        return -1;

    const auto* hdr = reinterpret_cast<const S_NFSTextureHeader*>(headerData.data());
    ConstructHeader(hdr);
    m_Data = imageData;

    return 0;
}

int CNFSTexture::SaveAsDDS(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return 0;

    file.write(reinterpret_cast<const char*>(&MAGIC_DDS), sizeof(MAGIC_DDS));
    file.write(reinterpret_cast<const char*>(&m_DDSHeader), sizeof(m_DDSHeader));
    if (!m_Data.empty()) {
        file.write(reinterpret_cast<const char*>(m_Data.data()), m_Data.size());
    }

    return 1;
}

int CNFSTexture::GetNumComponents() const
{
    return (m_DDSHeader.ddspf.dwFourCC == FOURCC_DXT1) ? 3 : 4;
}

int CNFSTexture::GetWidth() const
{
    return static_cast<int>(m_DDSHeader.dwWidth);
}

int CNFSTexture::GetHeight() const
{
    return static_cast<int>(m_DDSHeader.dwHeight);
}

int CNFSTexture::GetDecompressedSize() const
{
    return GetWidth() * GetHeight() * GetNumComponents();
}

int CNFSTexture::Decompress(uint8* data)
{
    if (!data || m_Data.empty() || m_DDSHeader.dwWidth == 0 || m_DDSHeader.dwHeight == 0)
        return 0;

    if (m_DDSHeader.ddspf.dwFlags == DDS_RGBA)
    {
        const size_t expectedSize = static_cast<size_t>(m_DDSHeader.dwWidth) * m_DDSHeader.dwHeight * 4;
        const size_t copySize = std::min(expectedSize, m_Data.size());
        std::memcpy(data, m_Data.data(), copySize);
    }
    else if (m_DDSHeader.ddspf.dwFlags & DDS_FOURCC)
    {
        DDS_DecodeCompressedImage(data, m_Data.data(), m_DDSHeader.dwWidth, m_DDSHeader.dwHeight, m_DDSHeader.ddspf.dwFourCC);
    }
    else
    {
        return 0; // Неизвестный формат - не трогаем память!
    }

    return GetDecompressedSize();
}

void CNFSTexture::ConstructHeader(const S_NFSTextureHeader* hdr)
{
    m_DDSHeader = {};

    m_DDSHeader.dwSize          = sizeof(m_DDSHeader);
    m_DDSHeader.dwFlags         = DDS_CAPS | DDS_WIDTH | DDS_HEIGHT | DDS_MIPMAPCOUNT;
    m_DDSHeader.dwWidth         = hdr->width;
    m_DDSHeader.dwHeight        = hdr->height;
    m_DDSHeader.dwCaps1         = DDS_TEXTURE;
    m_DDSHeader.dwMipMapCount   = hdr->numMipmaps;

    m_DDSHeader.ddspf.dwSize    = sizeof(m_DDSHeader.ddspf);

    if (hdr->type == FOURCC_DXT1 || hdr->type == FOURCC_DXT5)
    {
        m_DDSHeader.ddspf.dwFlags   = DDS_FOURCC;
        m_DDSHeader.ddspf.dwFourCC  = hdr->type;
    }
    else if (hdr->type == 21) // RGBA
    {
        m_DDSHeader.ddspf.dwFlags       = DDS_RGBA;
        m_DDSHeader.ddspf.dwRGBBitCount = 32;
        m_DDSHeader.ddspf.dwRBitMask    = 0x00ff0000;
        m_DDSHeader.ddspf.dwGBitMask    = 0x0000ff00;
        m_DDSHeader.ddspf.dwBBitMask    = 0x000000ff;
        m_DDSHeader.ddspf.dwABitMask    = 0xff000000;
    }
}