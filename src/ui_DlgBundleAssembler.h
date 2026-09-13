/********************************************************************************
** Form generated from reading UI file 'DlgBundleAssembler.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGBUNDLEASSEMBLER_H
#define UI_DLGBUNDLEASSEMBLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_DlgBundleAssembler
{
public:
    QFrame *frame;
    QPushButton *btnAssemble;
    QPushButton *btnRefresh;
    QPushButton *btnCancel;
    QListWidget *lstBundles;
    QListWidget *lstFilesToReplace;
    QLabel *label;
    QLabel *label_2;
    QGroupBox *groupBox;
    QCheckBox *chbCreateBackup;
    QToolButton *btnAddBundlesDir;
    QToolButton *btnAddFilesDir;
    QFrame *line;
    QToolButton *btnBundlesClear;
    QToolButton *btnBundlesClear_2;

    void setupUi(QDialog *DlgBundleAssembler)
    {
        if (DlgBundleAssembler->objectName().isEmpty())
            DlgBundleAssembler->setObjectName(QString::fromUtf8("DlgBundleAssembler"));
        DlgBundleAssembler->setWindowModality(Qt::WindowModal);
        DlgBundleAssembler->resize(711, 547);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DlgBundleAssembler->sizePolicy().hasHeightForWidth());
        DlgBundleAssembler->setSizePolicy(sizePolicy);
        DlgBundleAssembler->setMinimumSize(QSize(711, 547));
        DlgBundleAssembler->setMaximumSize(QSize(711, 547));
        DlgBundleAssembler->setModal(true);
        frame = new QFrame(DlgBundleAssembler);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 490, 711, 61));
        frame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"	border: 1px solid;\n"
"	background-color: rgb(255, 255, 255);\n"
"	border-top-color: rgb(0, 0, 0);\n"
"	border-right-color: rgb(255, 255, 255);\n"
"	border-bottom-color: rgb(255, 255, 255);\n"
"	border-left-color: rgb(255, 255, 255);\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        btnAssemble = new QPushButton(frame);
        btnAssemble->setObjectName(QString::fromUtf8("btnAssemble"));
        btnAssemble->setEnabled(true);
        btnAssemble->setGeometry(QRect(590, 14, 111, 31));
        btnRefresh = new QPushButton(frame);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));
        btnRefresh->setGeometry(QRect(470, 14, 111, 31));
        btnCancel = new QPushButton(frame);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(10, 14, 111, 31));
        lstBundles = new QListWidget(DlgBundleAssembler);
        lstBundles->setObjectName(QString::fromUtf8("lstBundles"));
        lstBundles->setGeometry(QRect(4, 25, 256, 461));
        lstFilesToReplace = new QListWidget(DlgBundleAssembler);
        lstFilesToReplace->setObjectName(QString::fromUtf8("lstFilesToReplace"));
        lstFilesToReplace->setGeometry(QRect(278, 25, 256, 461));
        label = new QLabel(DlgBundleAssembler);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(4, 5, 141, 16));
        label_2 = new QLabel(DlgBundleAssembler);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(278, 5, 141, 16));
        groupBox = new QGroupBox(DlgBundleAssembler);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(550, 19, 151, 467));
        groupBox->setAlignment(Qt::AlignCenter);
        chbCreateBackup = new QCheckBox(groupBox);
        chbCreateBackup->setObjectName(QString::fromUtf8("chbCreateBackup"));
        chbCreateBackup->setGeometry(QRect(10, 20, 131, 17));
        chbCreateBackup->setChecked(true);
        btnAddBundlesDir = new QToolButton(DlgBundleAssembler);
        btnAddBundlesDir->setObjectName(QString::fromUtf8("btnAddBundlesDir"));
        btnAddBundlesDir->setGeometry(QRect(236, 3, 25, 19));
        btnAddFilesDir = new QToolButton(DlgBundleAssembler);
        btnAddFilesDir->setObjectName(QString::fromUtf8("btnAddFilesDir"));
        btnAddFilesDir->setGeometry(QRect(510, 3, 25, 19));
        line = new QFrame(DlgBundleAssembler);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(261, 3, 16, 482));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        btnBundlesClear = new QToolButton(DlgBundleAssembler);
        btnBundlesClear->setObjectName(QString::fromUtf8("btnBundlesClear"));
        btnBundlesClear->setGeometry(QRect(198, 3, 31, 20));
        btnBundlesClear_2 = new QToolButton(DlgBundleAssembler);
        btnBundlesClear_2->setObjectName(QString::fromUtf8("btnBundlesClear_2"));
        btnBundlesClear_2->setGeometry(QRect(472, 3, 31, 20));

        retranslateUi(DlgBundleAssembler);

        QMetaObject::connectSlotsByName(DlgBundleAssembler);
    } // setupUi

    void retranslateUi(QDialog *DlgBundleAssembler)
    {
        DlgBundleAssembler->setWindowTitle(QCoreApplication::translate("DlgBundleAssembler", "NFS:HP Bundle Assembler", nullptr));
        btnAssemble->setText(QCoreApplication::translate("DlgBundleAssembler", "Assemble !", nullptr));
        btnRefresh->setText(QCoreApplication::translate("DlgBundleAssembler", "Refresh list", nullptr));
        btnCancel->setText(QCoreApplication::translate("DlgBundleAssembler", "Cancel", nullptr));
        label->setText(QCoreApplication::translate("DlgBundleAssembler", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Bundles list:</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("DlgBundleAssembler", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Files to replace:</span></p></body></html>", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DlgBundleAssembler", "Options:", nullptr));
        chbCreateBackup->setText(QCoreApplication::translate("DlgBundleAssembler", "Create bundle backup", nullptr));
#if QT_CONFIG(tooltip)
        btnAddBundlesDir->setToolTip(QCoreApplication::translate("DlgBundleAssembler", "Choose bundle to assemble...", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAddBundlesDir->setText(QCoreApplication::translate("DlgBundleAssembler", "...", nullptr));
#if QT_CONFIG(tooltip)
        btnAddFilesDir->setToolTip(QCoreApplication::translate("DlgBundleAssembler", "Choose files for assembling...", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAddFilesDir->setText(QCoreApplication::translate("DlgBundleAssembler", "...", nullptr));
#if QT_CONFIG(tooltip)
        btnBundlesClear->setToolTip(QCoreApplication::translate("DlgBundleAssembler", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Clear bundles list</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btnBundlesClear->setText(QCoreApplication::translate("DlgBundleAssembler", "clear", nullptr));
#if QT_CONFIG(tooltip)
        btnBundlesClear_2->setToolTip(QCoreApplication::translate("DlgBundleAssembler", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Clear files list</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btnBundlesClear_2->setText(QCoreApplication::translate("DlgBundleAssembler", "clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgBundleAssembler: public Ui_DlgBundleAssembler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGBUNDLEASSEMBLER_H
