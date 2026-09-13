#include "NFSBundle.h"

CNFSBundle::CNFSBundle() = default;

CNFSBundle::~CNFSBundle() = default;

int CNFSBundle::InitFromFile(const QString& fileName)
{
    // Старый файл автоматически уничтожится, память освободится
    m_MappedFile = std::make_unique<QtMappedFile>(fileName);
    m_MappedFile->ReadData(&m_Header, sizeof(m_Header));

    m_FilesTable.resize(m_Header.numFiles);
    m_MappedFile->ReadData(m_FilesTable.data(), sizeof(S_NFSTableEntry) * m_Header.numFiles);

    GatherFilesInfos();

    return 1;
}

int CNFSBundle::GetFilesCount() const
{
    return static_cast<int>(m_FilesTable.size());
}

const S_NFSFileInfo& CNFSBundle::GetFileInfo(int idx) const
{
    return m_FilesInfos[idx];
}

const S_NFSTableEntry& CNFSBundle::GetFileEntry(int idx) const
{
    return m_FilesTable[idx];
}

const S_NFSBndlHeader& CNFSBundle::GetBundleHeader() const
{
    return m_Header;
}

void* CNFSBundle::GetDataAtOffset(size_t offset)
{
    return m_MappedFile ? m_MappedFile->GetDataAtOffset(offset) : nullptr;
}

void CNFSBundle::GatherFilesInfos()
{
    const size_t count = m_FilesTable.size();
    m_FilesInfos.resize(count);

    for (size_t i = 0; i < count; ++i)
    {
        const auto& src = m_FilesTable[i];
        auto& dst       = m_FilesInfos[i];

        dst.fileHeaderOffset = src.hdrOffset + m_Header.headersOffset;
        dst.fileBodyOffset   = src.bodyOffset + m_Header.bodysOffset;
        dst.hdrUncompSize    = src.hdrUncompSize & k_EntryUncompSizeMask;
        dst.dataUncompSize   = src.dataUncompSize & k_EntryUncompSizeMask;
        dst.hdrCompSize      = src.hdrCompSize;
        dst.dataCompSize     = src.dataCompSize;
        dst.fileType         = src.fileType;
    }
}