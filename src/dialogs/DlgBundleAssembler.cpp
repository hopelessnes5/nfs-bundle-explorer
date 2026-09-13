#include "DlgBundleAssembler.h"
#include "ui_DlgBundleAssembler.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <vector>
#include <memory>

#include "../zlib_adapter.h"

DlgBundleAssembler::DlgBundleAssembler(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::DlgBundleAssembler>())
{
    ui->setupUi(this);
}

DlgBundleAssembler::~DlgBundleAssembler() = default;

bool DlgBundleAssembler::CheckForCorrectNFSFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    uint32 nfsoMagic = 0;
    uint32 fileType = 0;
    if (sizeof(nfsoMagic) != file.read(reinterpret_cast<char*>(&nfsoMagic), sizeof(nfsoMagic)))
        return false;
    if (sizeof(fileType) != file.read(reinterpret_cast<char*>(&fileType), sizeof(fileType)))
        return false;

    return (nfsoMagic == MAKEFOURCC('N', 'F', 'S', 'O') && fileType == NFS_FILE_TEXTURE);
}

int DlgBundleAssembler::AssembleBundle(const QString& bundlePath, const QList<QString>& filesList)
{
    auto bundle = std::make_unique<CNFSBundle>();
    if (!bundle->InitFromFile(bundlePath)) {
        return 0;
    }

    QFile dest(bundlePath + QStringLiteral(".temp"));
    if (!dest.open(QIODevice::WriteOnly)) {
        return 0;
    }

    S_NFSBndlHeader header = bundle->GetBundleHeader();
    dest.write(reinterpret_cast<const char*>(&header), sizeof(header));

    const int filesCount = bundle->GetFilesCount();
    std::vector<S_NFSFileInfo> preparedList;
    preparedList.reserve(filesCount);

    // Первый проход - таблица файлов
    uint32 relativeHeaderOffset = 0;
    uint32 relativeBodyOffset = 0;
    for (int i = 0; i < filesCount; ++i)
    {
        preparedList.push_back(bundle->GetFileInfo(i));
        S_NFSFileInfo& info = preparedList.back();

        QString fileStr = QStringLiteral("file_%1.nfs").arg(i, 4, 10, QChar('0'));
        if (filesList.contains(fileStr))
        {
            QFile replFile(m_FilesMap[fileStr]);
            if (!replFile.open(QIODevice::ReadOnly)) {
                return 0;
            }

            uint32 nfsoMagic = 0;
            uint32 nfsFileType = 0;
            replFile.read(reinterpret_cast<char*>(&nfsoMagic), sizeof(nfsoMagic));
            replFile.read(reinterpret_cast<char*>(&nfsFileType), sizeof(nfsFileType));

            if (nfsFileType == NFS_FILE_TEXTURE)
            {
                if (!PrepareTextureFile(replFile, info))
                    return 0;
            }
        }

        S_NFSTableEntry entry = bundle->GetFileEntry(i);
        entry.hdrCompSize = info.hdrCompSize;
        entry.hdrUncompSize = (info.hdrUncompSize & k_EntryUncompSizeMask) | (entry.hdrUncompSize & ~k_EntryUncompSizeMask);
        entry.dataCompSize = info.dataCompSize;
        entry.dataUncompSize = (info.dataUncompSize & k_EntryUncompSizeMask) | (entry.dataUncompSize & ~k_EntryUncompSizeMask);
        entry.hdrOffset = relativeHeaderOffset;
        entry.bodyOffset = info.dataCompSize > 0 ? relativeBodyOffset : 0;
        dest.write(reinterpret_cast<const char*>(&entry), sizeof(entry));

        relativeHeaderOffset += GetAligned(info.hdrCompSize, k_RegularAllocationAlign);
        relativeBodyOffset += GetAligned(info.dataCompSize, k_BigAllocationAlign);
    }

    constexpr char nulls[128] = {0};

    // Второй проход - заголовки файлов
    uint32 baseHeaderOffset = header.headersOffset;
    for (int i = 0; i < filesCount; ++i)
    {
        const auto& info = preparedList[i];
        QString fileStr = QStringLiteral("file_%1.nfs").arg(i, 4, 10, QChar('0'));

        if (filesList.contains(fileStr))
        {
            QString hdrFileName = m_FilesMap[fileStr] + QStringLiteral("c_hdr");
            QFile file(hdrFileName);
            if (!file.open(QIODevice::ReadOnly))
                return 0;

            std::vector<char> data(file.size());
            file.read(data.data(), file.size());
            file.close();

            dest.write(data.data(), data.size());
            uint32 alignedSize = GetAligned(info.hdrCompSize, k_RegularAllocationAlign);
            uint32 paddingLen = alignedSize - info.hdrCompSize;
            if (paddingLen > 0)
                dest.write(nulls, paddingLen);

            baseHeaderOffset += alignedSize;
        }
        else
        {
            const auto* data = reinterpret_cast<const char*>(bundle->GetDataAtOffset(info.fileHeaderOffset));
            dest.write(data, info.hdrCompSize);
            uint32 alignedSize = GetAligned(info.hdrCompSize, k_RegularAllocationAlign);
            uint32 paddingLen = alignedSize - info.hdrCompSize;
            if (paddingLen > 0)
                dest.write(nulls, paddingLen);

            baseHeaderOffset += alignedSize;
        }
    }

    uint32 alignedHeaderOffset = GetAligned(baseHeaderOffset, k_BigAllocationAlign);
    uint32 headerToBodyPadding = alignedHeaderOffset - baseHeaderOffset;
    if (headerToBodyPadding > 0)
        dest.write(nulls, headerToBodyPadding);

    // Третий проход - тела файлов
    uint32 baseBodiesOffset = alignedHeaderOffset;
    for (int i = 0; i < filesCount; ++i)
    {
        const auto& info = preparedList[i];
        if (info.dataCompSize == 0) continue;

        QString fileStr = QStringLiteral("file_%1.nfs").arg(i, 4, 10, QChar('0'));
        if (filesList.contains(fileStr))
        {
            QString bodyFileName = m_FilesMap[fileStr] + QStringLiteral("c_body");
            QFile file(bodyFileName);
            if (!file.open(QIODevice::ReadOnly))
                return 0;

            std::vector<char> data(file.size());
            file.read(data.data(), file.size());
            file.close();

            dest.write(data.data(), data.size());
            uint32 alignedSize = GetAligned(info.dataCompSize, k_BigAllocationAlign);
            uint32 paddingLen = alignedSize - info.dataCompSize;
            if (paddingLen > 0)
                dest.write(nulls, paddingLen);

            baseBodiesOffset += alignedSize;
        }
        else
        {
            const auto* data = reinterpret_cast<const char*>(bundle->GetDataAtOffset(info.fileBodyOffset));
            dest.write(data, info.dataCompSize);
            uint32 alignedSize = GetAligned(info.dataCompSize, k_BigAllocationAlign);
            uint32 paddingLen = alignedSize - info.dataCompSize;
            if (paddingLen > 0)
                dest.write(nulls, paddingLen);

            baseBodiesOffset += alignedSize;
        }
    }

    // Корректировка заголовка
    header.bodysOffset = alignedHeaderOffset;
    header.bundleSize = header.bundleSize2 = header.dataEnd = baseBodiesOffset;
    dest.seek(0);
    dest.write(reinterpret_cast<const char*>(&header), sizeof(header));

    dest.close();
    return 1;
}

int DlgBundleAssembler::PrepareTextureFile(QFile& file, S_NFSFileInfo& info) const
{
    S_NFSTextureHeader header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    Uint8Vector hdrCompressed;
    if (zlib_adapter::CompressMemory(&header, sizeof(header), hdrCompressed) <= 0)
        return 0;

    info.hdrUncompSize = sizeof(header);
    info.hdrCompSize = static_cast<uint32>(hdrCompressed.size());

    uint32 fileBodySize = static_cast<uint32>(file.size() - file.pos());
    std::vector<char> fileBody(fileBodySize);
    file.read(fileBody.data(), fileBodySize);

    Uint8Vector bodyCompressed;
    if (zlib_adapter::CompressMemory(fileBody.data(), fileBodySize, bodyCompressed) <= 0)
        return 0;

    info.dataUncompSize = fileBodySize;
    info.dataCompSize = static_cast<uint32>(bodyCompressed.size());

    QString hdrFileName = file.fileName() + QStringLiteral("c_hdr");
    QString bodyFileName = file.fileName() + QStringLiteral("c_body");

    QFile hdrFile(hdrFileName);
    if (!hdrFile.open(QIODevice::WriteOnly))
        return 0;
    hdrFile.write(reinterpret_cast<const char*>(hdrCompressed.data()), hdrCompressed.size());
    hdrFile.close();

    QFile bodyFile(bodyFileName);
    if (!bodyFile.open(QIODevice::WriteOnly))
        return 0;
    bodyFile.write(reinterpret_cast<const char*>(bodyCompressed.data()), bodyCompressed.size());
    bodyFile.close();

    return 1; // Исправлено: возвращаем 1 при успехе! (раньше тут не было return)
}

void DlgBundleAssembler::on_btnCancel_clicked()
{
    close();
}

void DlgBundleAssembler::on_btnAddBundlesDir_clicked()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Choose bundle file"), QString(), tr("NFS:HP bundle (*.bndl *.bundle *.bin)"));
    if (name.length() < 3)
        return;

    QFileInfo info(name);
    QString fileName = info.fileName();
    if (m_BundlesMap.contains(fileName))
    {
        QMessageBox::warning(this, tr("Warning"), tr("This bundle is already in the list!"));
        return;
    }

    auto* item = new QListWidgetItem(fileName);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setCheckState(Qt::Unchecked);
    ui->lstBundles->addItem(item);

    m_BundlesMap.insert(fileName, name);
}

void DlgBundleAssembler::on_lstBundles_itemChanged(QListWidgetItem* item)
{
    bool checked = (item->checkState() == Qt::Checked);
    const int count = ui->lstBundles->count();
    for (int i = 0; i < count; ++i)
    {
        QListWidgetItem* it = ui->lstBundles->item(i);
        if (it != item && checked)
            it->setCheckState(Qt::Unchecked);
    }
}

void DlgBundleAssembler::on_btnAddFilesDir_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Choose file for replace"), QString(), tr("NFS:HP files (*.nfs)"));
    if (files.isEmpty())
        return;

    for (const auto& filePath : files)
    {
        if (CheckForCorrectNFSFile(filePath))
        {
            QFileInfo info(filePath);
            QString fileName = info.fileName();

            if (!m_FilesMap.contains(fileName))
            {
                auto* item = new QListWidgetItem(fileName);
                item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                item->setCheckState(Qt::Unchecked);
                ui->lstFilesToReplace->addItem(item);

                m_FilesMap.insert(fileName, filePath);
            }
        }
    }
}

void DlgBundleAssembler::on_btnBundlesClear_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, tr("Question"), tr("Do you really want to clear bundles list?"), QMessageBox::Yes, QMessageBox::No))
    {
        ui->lstBundles->clear();
        m_BundlesMap.clear();
    }
}

void DlgBundleAssembler::on_btnBundlesClear_2_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, tr("Question"), tr("Do you really want to clear files list?"), QMessageBox::Yes, QMessageBox::No))
    {
        ui->lstFilesToReplace->clear();
        m_FilesMap.clear();
    }
}

void DlgBundleAssembler::on_btnAssemble_clicked()
{
    QListWidgetItem* bundleItem = nullptr;

    const int bundleCount = ui->lstBundles->count();
    for (int i = 0; i < bundleCount; ++i)
    {
        QListWidgetItem* it = ui->lstBundles->item(i);
        if (Qt::Checked == it->checkState())
            bundleItem = it;
    }

    if (!bundleItem)
    {
        QMessageBox::critical(this, tr("Error!"), tr("You must have checked bundle in the list"));
        return;
    }

    QList<QString> files;
    const int filesCount = ui->lstFilesToReplace->count();
    for (int i = 0; i < filesCount; ++i)
    {
        QListWidgetItem* it = ui->lstFilesToReplace->item(i);
        if (Qt::Checked == it->checkState())
            files.push_back(it->text());
    }

    if (files.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("You must have checked files in the list"));
        return;
    }

    AssembleBundle(m_BundlesMap[bundleItem->text()], files);
}