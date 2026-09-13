#ifndef __QtMappedFile_h__
#define __QtMappedFile_h__

#include <QFile>
#include <QString>
#include "common.h"

class QtMappedFile : public QFile
{
    Q_OBJECT

public:
    explicit QtMappedFile(QObject *parent = nullptr);
    explicit QtMappedFile(const QString& fileName);
    ~QtMappedFile() override;

    uint8   ReadUint8();
    uint32  ReadUint32();
    size_t  ReadData(void* data, size_t len);
    void*   GetDataAtOffset(size_t offset);
    void    SeekRelative(size_t bytesToSeek);

private:
    uint8*  m_Data   = nullptr;
    size_t  m_Offset = 0;
    size_t  m_Size   = 0;
};

#endif // __QtMappedFile_h__