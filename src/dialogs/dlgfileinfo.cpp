#include "dlgfileinfo.h"
#include "ui_dlgfileinfo.h"
#include <zlib.h>

DlgFileInfo::DlgFileInfo(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::DlgFileInfo>())
{
    ui->setupUi(this);
}

DlgFileInfo::~DlgFileInfo() = default;

namespace {

QString FormatSizeString(uint32 size)
{
    uint32 mb = size / (1024 * 1024);
    size -= mb * 1024 * 1024;
    uint32 kb = size / 1024;
    size -= kb * 1024;
    uint32 b  = size;

    return QStringLiteral("%1 MB %2 KB %3 B").arg(mb).arg(kb).arg(b);
}

QString FileTypeString(uint32 fileType)
{
    if (fileType == NFS_FILE_TEXTURE)
        return QStringLiteral("texture (%1)").arg(fileType);
    if (fileType == NFS_FILE_MODEL)
        return QStringLiteral("model (%1)").arg(fileType);
    return QStringLiteral("unknown (%1)").arg(fileType);
}

} // anonymous namespace

void DlgFileInfo::SetFileInfo(const S_NFSFileInfo& info, CNFSBundle* bundle)
{
    if (!bundle) return;

    ui->lblFileType->setText(FileTypeString(info.fileType));
    ui->lblFileHeaderPacked->setText(FormatSizeString(info.hdrCompSize));
    ui->lblFileHeaderUnpacked->setText(FormatSizeString(info.hdrUncompSize));
    ui->lblFileDataPacked->setText(FormatSizeString(info.dataCompSize));
    ui->lblFileDataUnpacked->setText(FormatSizeString(info.dataUncompSize));

    uint32 hdrCRC32 = 0;
    uint32 dataCRC32 = 0;
    uint32 fullCRC32 = 0;

    auto* data = reinterpret_cast<Bytef*>(bundle->GetDataAtOffset(info.fileHeaderOffset));
    if (data) {
        hdrCRC32 = crc32(0, data, info.hdrCompSize);
    }

    if (info.dataCompSize > 0)
    {
        data = reinterpret_cast<Bytef*>(bundle->GetDataAtOffset(info.fileBodyOffset));
        if (data) {
            dataCRC32 = crc32(0, data, info.dataCompSize);
            fullCRC32 = crc32(hdrCRC32, data, info.dataCompSize);
        }
    }
    else {
        fullCRC32 = hdrCRC32;
    }

    ui->lblFileHeaderCRC32->setText(QString::number(hdrCRC32, 16));
    ui->lblFileDataCRC32->setText(QString::number(dataCRC32, 16));
    ui->lblFullFileCRC32->setText(QString::number(fullCRC32, 16));
}

void DlgFileInfo::on_pushButton_clicked()
{
    close();
}