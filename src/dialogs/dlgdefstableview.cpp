#include "dlgdefstableview.h"
#include "ui_dlgdefstableview.h"
#include "../zlib_adapter.h"
#include <cstring>
#include <vector>

DlgDefsTableView::DlgDefsTableView(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::DlgDefsTableView>())
    , m_Bundle(nullptr)
    , m_Idx(-1)
{
    ui->setupUi(this);
    ui->tblTable->setColumnCount(3);
    ui->tblTable->setHorizontalHeaderLabels({QStringLiteral("Field ID"), QStringLiteral("Offset"), QStringLiteral("Type")});
}

DlgDefsTableView::~DlgDefsTableView() = default;

void DlgDefsTableView::SetBundleAndIdx(CNFSBundle* bundle, int idx)
{
    if (!bundle) return;

    m_Bundle = bundle;
    m_Idx = idx;

    const S_NFSFileInfo& info = bundle->GetFileInfo(idx);
    auto* compData = reinterpret_cast<uint8*>(bundle->GetDataAtOffset(info.fileHeaderOffset));

    std::vector<uint8> headerData(info.hdrUncompSize, 0);
    zlib_adapter::DecompressMemory(compData, info.hdrCompSize, headerData);

    if (headerData.size() < sizeof(S_NFSDefsTableHeader)) return;

    std::memcpy(&m_DefsTableHeader, headerData.data(), sizeof(S_NFSDefsTableHeader));

    const size_t totalEntriesSize = sizeof(S_NFSDefsTableEntry) * m_DefsTableHeader.defsCount;
    if (sizeof(S_NFSDefsTableHeader) + totalEntriesSize > headerData.size()) return;

    m_DefsTableEntries.resize(m_DefsTableHeader.defsCount);
    std::memcpy(m_DefsTableEntries.data(), headerData.data() + sizeof(S_NFSDefsTableHeader), totalEntriesSize);

    ui->lblDefTableName->setText(QString::fromLatin1(m_DefsTableHeader.defName));

    ui->tblTable->setRowCount(0);
    for (uint32 i = 0; i < m_DefsTableHeader.defsCount; ++i)
    {
        const auto& entry = m_DefsTableEntries[i];
        ui->tblTable->insertRow(i);

        ui->tblTable->setItem(i, 0, new QTableWidgetItem(QString::number(entry.defID, 16)));
        ui->tblTable->setItem(i, 1, new QTableWidgetItem(QString::number(entry.offset)));
        ui->tblTable->setItem(i, 2, new QTableWidgetItem(QString::number(entry.type)));
    }
}