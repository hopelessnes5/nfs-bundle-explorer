#ifndef __NFSBundle_h__
#define __NFSBundle_h__

#include <QString>
#include <vector>
#include <memory>
#include "QtMappedFile.h"
#include "common.h"

struct S_NFSFileInfo
{
    uint32  fileHeaderOffset;
    uint32  fileBodyOffset;
    uint32  hdrUncompSize;
    uint32  dataUncompSize;
    uint32  hdrCompSize;
    uint32  dataCompSize;
    uint32  fileType;
};

class CNFSBundle
{
public:
    CNFSBundle();
    ~CNFSBundle();

    // Запрещаем случайное копирование всего бандла в памяти
    CNFSBundle(const CNFSBundle&) = delete;
    CNFSBundle& operator=(const CNFSBundle&) = delete;

    // Разрешаем быстрое перемещение (move)
    CNFSBundle(CNFSBundle&&) noexcept = default;
    CNFSBundle& operator=(CNFSBundle&&) noexcept = default;

    int InitFromFile(const QString& fileName);

    [[nodiscard]] int GetFilesCount() const;
    [[nodiscard]] const S_NFSFileInfo&   GetFileInfo(int idx) const;
    [[nodiscard]] const S_NFSTableEntry& GetFileEntry(int idx) const;
    [[nodiscard]] const S_NFSBndlHeader& GetBundleHeader() const;

    void* GetDataAtOffset(size_t offset);

private:
    void GatherFilesInfos();

private:
    S_NFSBndlHeader                 m_Header{};
    std::vector<S_NFSTableEntry>    m_FilesTable;
    std::vector<S_NFSFileInfo>      m_FilesInfos;
    std::unique_ptr<QtMappedFile>   m_MappedFile;
};

#endif // __NFSBundle_h__