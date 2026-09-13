/*
 * This code is written by iOrange, 2010
 * Modernized for C++17 & Qt 5/6 (2024)
 */

#ifndef DLGFILEINFO_H
#define DLGFILEINFO_H

#include <QDialog>
#include <memory>
#include "../common.h"
#include "../NFSBundle.h"

namespace Ui {
    class DlgFileInfo;
}

class DlgFileInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFileInfo(QWidget *parent = nullptr);
    ~DlgFileInfo() override;

    void SetFileInfo(const S_NFSFileInfo& info, CNFSBundle* bundle);

private:
    std::unique_ptr<Ui::DlgFileInfo> ui;

private slots:
    void on_pushButton_clicked();
};

#endif // DLGFILEINFO_H