/********************************************************************************
** Form generated from reading UI file 'dlgdefstableview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGDEFSTABLEVIEW_H
#define UI_DLGDEFSTABLEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgDefsTableView
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QLabel *label;
    QLabel *lblDefTableName;
    QTableWidget *tblTable;

    void setupUi(QDialog *DlgDefsTableView)
    {
        if (DlgDefsTableView->objectName().isEmpty())
            DlgDefsTableView->setObjectName(QString::fromUtf8("DlgDefsTableView"));
        DlgDefsTableView->resize(970, 654);
        verticalLayout = new QVBoxLayout(DlgDefsTableView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(DlgDefsTableView);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(500, 32));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 121, 16));
        lblDefTableName = new QLabel(frame);
        lblDefTableName->setObjectName(QString::fromUtf8("lblDefTableName"));
        lblDefTableName->setGeometry(QRect(130, 10, 421, 16));

        verticalLayout->addWidget(frame);

        tblTable = new QTableWidget(DlgDefsTableView);
        tblTable->setObjectName(QString::fromUtf8("tblTable"));
        tblTable->setMinimumSize(QSize(500, 500));
        tblTable->verticalHeader()->setMinimumSectionSize(10);
        tblTable->verticalHeader()->setDefaultSectionSize(17);

        verticalLayout->addWidget(tblTable);


        retranslateUi(DlgDefsTableView);

        QMetaObject::connectSlotsByName(DlgDefsTableView);
    } // setupUi

    void retranslateUi(QDialog *DlgDefsTableView)
    {
        DlgDefsTableView->setWindowTitle(QCoreApplication::translate("DlgDefsTableView", "NFS:HP 2010 - Definitions table viewer", nullptr));
        label->setText(QCoreApplication::translate("DlgDefsTableView", "Definitions table name:", nullptr));
        lblDefTableName->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgDefsTableView: public Ui_DlgDefsTableView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGDEFSTABLEVIEW_H
