#include "QtMappedFile.h"
#include <cstring>
#include <algorithm>

QtMappedFile::QtMappedFile(QObject *parent)
    : QFile(parent)
    , m_Data(nullptr)
    , m_Offset(0)
    , m_Size(0)
{
}

QtMappedFile::QtMappedFile(const QString& fileName)
    : QFile(fileName)
    , m_Data(nullptr)
    , m_Offset(0)
    , m_Size(0)
{
    if (QFile::open(QIODevice::ReadOnly))
    {
        m_Size = static_cast<size_t>(QFile::size());
        m_Data = QFile::map(0, static_cast<qint64>(m_Size));
    }
}

QtMappedFile::~QtMappedFile()
{
    if (m_Data) {
        QFile::unmap(m_Data);
        m_Data = nullptr;
    }
    if (QFile::isOpen()) {
        QFile::close();
    }
}

uint8 QtMappedFile::ReadUint8()
{
    if (!m_Data || m_Offset >= m_Size) {
        return 0;
    }

    uint8 u = m_Data[m_Offset]; // Исправлено: теперь читается с учетом смещения
    m_Offset++;
    return u;
}

uint32 QtMappedFile::ReadUint32()
{
    uint32 u = 0;
    ReadData(&u, sizeof(u));
    return u;
}

size_t QtMappedFile::ReadData(void* data, size_t len)
{
    if (!m_Data || m_Offset >= m_Size || !data || len == 0) {
        return 0;
    }

    // Защита от переполнения: читаем ровно столько, сколько осталось в файле
    size_t bytesToRead = std::min(len, m_Size - m_Offset);
    std::memcpy(data, m_Data + m_Offset, bytesToRead);
    m_Offset += bytesToRead;
    return bytesToRead;
}

void* QtMappedFile::GetDataAtOffset(size_t offset)
{
    if (!m_Data || offset >= m_Size) {
        return nullptr;
    }
    return m_Data + offset;
}

void QtMappedFile::SeekRelative(size_t bytesToSeek)
{
    m_Offset = std::min(m_Offset + bytesToSeek, m_Size);
}