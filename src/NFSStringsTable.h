#ifndef NFSSTRINGSTABLE_H
#define NFSSTRINGSTABLE_H

#include <QString>
#include <vector>
#include "common.h"

class CNFSStringsTable
{
public:
    CNFSStringsTable();
    ~CNFSStringsTable();

    [[nodiscard]] bool InitFromData(const Uint8Vector& data);
    [[nodiscard]] bool SaveAsTXT(const QString& fileName);

private:
    using QStringsVec = std::vector<QString>;

private:
    S_NFSStringsTableHeader m_Header{};
    QStringsVec             m_Strings;
};

#endif // NFSSTRINGSTABLE_H