/*
 * This code is written by iOrange, 2010
 * Modernized for C++17 & Qt 5/6 (2024)
 */

#ifndef DLGBUNDLEASSEMBLER_H
#define DLGBUNDLEASSEMBLER_H

#include <QDialog>
#include <QListWidgetItem>
#include <QFile>
#include <QMap>
#include <memory>

#include "../NFSBundle.h"
#include "../common.h"

namespace Ui {
    class DlgBundleAssembler;
}

class DlgBundleAssembler : public QDialog
{
    Q_OBJECT

public:
    explicit DlgBundleAssembler(QWidget *parent = nullptr);
    ~DlgBundleAssembler() override;

private:
    using FilesMap = QMap<QString, QString>;

    std::unique_ptr<Ui::DlgBundleAssembler> ui;
    FilesMap m_FilesMap;
    FilesMap m_BundlesMap;

private:
    bool CheckForCorrectNFSFile(const QString& fileName);
    int  AssembleBundle(const QString& bundlePath, const QList<QString>& filesList);
    int  PrepareTextureFile(QFile& file, S_NFSFileInfo& info) const;

private slots:
    void on_btnAssemble_clicked();
    void on_btnBundlesClear_2_clicked();
    void on_btnBundlesClear_clicked();
    void on_btnAddFilesDir_clicked();
    void on_lstBundles_itemChanged(QListWidgetItem* item);
    void on_btnAddBundlesDir_clicked();
    void on_btnCancel_clicked();
};

#endif // DLGBUNDLEASSEMBLER_H