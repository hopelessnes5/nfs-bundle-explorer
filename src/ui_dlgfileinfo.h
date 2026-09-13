/********************************************************************************
** Form generated from reading UI file 'dlgfileinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGFILEINFO_H
#define UI_DLGFILEINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DlgFileInfo
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *lblFileType;
    QGroupBox *groupBox;
    QLabel *label_3;
    QLabel *lblFileHeaderPacked;
    QLabel *lblFileHeaderUnpacked;
    QLabel *label_6;
    QLabel *lblFileDataUnpacked;
    QLabel *lblFileDataPacked;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *lblFileHeaderCRC32;
    QLabel *lblFileDataCRC32;
    QLabel *label_4;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *lblFullFileCRC32;

    void setupUi(QDialog *DlgFileInfo)
    {
        if (DlgFileInfo->objectName().isEmpty())
            DlgFileInfo->setObjectName(QString::fromUtf8("DlgFileInfo"));
        DlgFileInfo->resize(685, 259);
        DlgFileInfo->setMinimumSize(QSize(685, 259));
        DlgFileInfo->setMaximumSize(QSize(685, 259));
        pushButton = new QPushButton(DlgFileInfo);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(600, 220, 75, 23));
        label = new QLabel(DlgFileInfo);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(610, 20, 48, 48));
        label->setPixmap(QPixmap(QString::fromUtf8(":/resources/icons/Information_48x48.png")));
        label_2 = new QLabel(DlgFileInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 20, 46, 16));
        lblFileType = new QLabel(DlgFileInfo);
        lblFileType->setObjectName(QString::fromUtf8("lblFileType"));
        lblFileType->setGeometry(QRect(70, 20, 381, 16));
        groupBox = new QGroupBox(DlgFileInfo);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 50, 421, 191));
        groupBox->setFlat(false);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 30, 141, 16));
        lblFileHeaderPacked = new QLabel(groupBox);
        lblFileHeaderPacked->setObjectName(QString::fromUtf8("lblFileHeaderPacked"));
        lblFileHeaderPacked->setGeometry(QRect(160, 30, 251, 16));
        lblFileHeaderUnpacked = new QLabel(groupBox);
        lblFileHeaderUnpacked->setObjectName(QString::fromUtf8("lblFileHeaderUnpacked"));
        lblFileHeaderUnpacked->setGeometry(QRect(160, 50, 251, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 50, 141, 16));
        lblFileDataUnpacked = new QLabel(groupBox);
        lblFileDataUnpacked->setObjectName(QString::fromUtf8("lblFileDataUnpacked"));
        lblFileDataUnpacked->setGeometry(QRect(160, 90, 251, 16));
        lblFileDataPacked = new QLabel(groupBox);
        lblFileDataPacked->setObjectName(QString::fromUtf8("lblFileDataPacked"));
        lblFileDataPacked->setGeometry(QRect(160, 70, 251, 16));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 90, 141, 16));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 70, 141, 16));
        lblFileHeaderCRC32 = new QLabel(groupBox);
        lblFileHeaderCRC32->setObjectName(QString::fromUtf8("lblFileHeaderCRC32"));
        lblFileHeaderCRC32->setGeometry(QRect(160, 110, 251, 16));
        lblFileDataCRC32 = new QLabel(groupBox);
        lblFileDataCRC32->setObjectName(QString::fromUtf8("lblFileDataCRC32"));
        lblFileDataCRC32->setGeometry(QRect(160, 130, 251, 16));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 110, 141, 16));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 130, 141, 16));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 150, 141, 16));
        lblFullFileCRC32 = new QLabel(groupBox);
        lblFullFileCRC32->setObjectName(QString::fromUtf8("lblFullFileCRC32"));
        lblFullFileCRC32->setGeometry(QRect(160, 150, 251, 16));

        retranslateUi(DlgFileInfo);

        QMetaObject::connectSlotsByName(DlgFileInfo);
    } // setupUi

    void retranslateUi(QDialog *DlgFileInfo)
    {
        DlgFileInfo->setWindowTitle(QCoreApplication::translate("DlgFileInfo", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("DlgFileInfo", "OK", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("DlgFileInfo", "File type:", nullptr));
        lblFileType->setText(QCoreApplication::translate("DlgFileInfo", "text", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DlgFileInfo", "File sizes info", nullptr));
        label_3->setText(QCoreApplication::translate("DlgFileInfo", "File header packed size:", nullptr));
        lblFileHeaderPacked->setText(QCoreApplication::translate("DlgFileInfo", "lblFileHeaderPacked", nullptr));
        lblFileHeaderUnpacked->setText(QCoreApplication::translate("DlgFileInfo", "lblFileHeaderUnpacked", nullptr));
        label_6->setText(QCoreApplication::translate("DlgFileInfo", "File header unpacked size:", nullptr));
        lblFileDataUnpacked->setText(QCoreApplication::translate("DlgFileInfo", "lblFileDataUnpacked", nullptr));
        lblFileDataPacked->setText(QCoreApplication::translate("DlgFileInfo", "lblFileDataPacked", nullptr));
        label_7->setText(QCoreApplication::translate("DlgFileInfo", "File data unpacked size:", nullptr));
        label_8->setText(QCoreApplication::translate("DlgFileInfo", "File data packed size:", nullptr));
        lblFileHeaderCRC32->setText(QCoreApplication::translate("DlgFileInfo", "lblFileHeaderCRC32", nullptr));
        lblFileDataCRC32->setText(QCoreApplication::translate("DlgFileInfo", "lblFileDataCRC32", nullptr));
        label_4->setText(QCoreApplication::translate("DlgFileInfo", "File header CRC32:", nullptr));
        label_9->setText(QCoreApplication::translate("DlgFileInfo", "File data CRC32:", nullptr));
        label_10->setText(QCoreApplication::translate("DlgFileInfo", "Full file CRC32:", nullptr));
        lblFullFileCRC32->setText(QCoreApplication::translate("DlgFileInfo", "lblFullFileCRC32", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgFileInfo: public Ui_DlgFileInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGFILEINFO_H
