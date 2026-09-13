#include "NFSStringsTable.h"
#include <QFile>
#include <QTextStream>
#include <cstring>
#include <vector>

CNFSStringsTable::CNFSStringsTable() = default;
CNFSStringsTable::~CNFSStringsTable() = default;

bool CNFSStringsTable::InitFromData(const Uint8Vector& data)
{
    if (data.size() < sizeof(S_NFSStringsTableHeader)) {
        return false;
    }

    uint32 offset = 0;
    std::memcpy(&m_Header, data.data() + offset, sizeof(S_NFSStringsTableHeader));
    offset += m_Header.stringInfosOffset;

    const size_t totalInfosSize = sizeof(S_NFSStringsTableInfo) * m_Header.stringsCount;
    if (offset + totalInfosSize > data.size()) {
        return false;
    }

    std::vector<S_NFSStringsTableInfo> infos(m_Header.stringsCount);
    std::memcpy(infos.data(), data.data() + offset, totalInfosSize);

    uint32 maxLen = 0;
    for (const auto& info : infos) {
        if (info.stringLength > maxLen) {
            maxLen = info.stringLength;
        }
    }

    // Безопасный буфер под строки UTF-16
    std::vector<char16_t> tempString(maxLen + 8, 0);
    m_Strings.resize(m_Header.stringsCount);

    for (uint32 i = 0; i < m_Header.stringsCount; ++i)
    {
        const auto& info = infos[i];
        size_t bytesToCopy = info.stringLength * (m_Header.charSize >> 3);

        if (info.offset + bytesToCopy <= data.size())
        {
            std::memcpy(tempString.data(), data.data() + info.offset, bytesToCopy);
            m_Strings[i] = QString::fromUtf16(tempString.data(), info.stringLength);
        }
    }

    return true;
}

bool CNFSStringsTable::SaveAsTXT(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    stream.setCodec("UTF-8");
#endif

    for (const auto& str : m_Strings) {
        stream << str << "\n";
    }

    stream.flush();
    return true;
}