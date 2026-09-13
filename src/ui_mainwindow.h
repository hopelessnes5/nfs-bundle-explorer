/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *actionE_xit;
    QAction *actionExtract_selected;
    QAction *actionExtract_all;
    QAction *actionRAW_data_extract_selected;
    QAction *actionRAW_data_extract_all;
    QAction *actionShow_file_information;
    QAction *actionAbout;
    QAction *actionDDS_NFS;
    QAction *actionBundle_assembler;
    QAction *actionClose_file;
    QAction *actionWrite_bundle_table;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QListWidget *listWidget;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuResource;
    QMenu *menuHelp;
    QMenu *menuTools;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(989, 693);
        MainWindow->setMinimumSize(QSize(800, 600));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/Positive_256x256.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/Open_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon1);
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/Remove_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionE_xit->setIcon(icon2);
        actionExtract_selected = new QAction(MainWindow);
        actionExtract_selected->setObjectName(QString::fromUtf8("actionExtract_selected"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/icons/Save_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExtract_selected->setIcon(icon3);
        actionExtract_all = new QAction(MainWindow);
        actionExtract_all->setObjectName(QString::fromUtf8("actionExtract_all"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/icons/Download_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExtract_all->setIcon(icon4);
        actionRAW_data_extract_selected = new QAction(MainWindow);
        actionRAW_data_extract_selected->setObjectName(QString::fromUtf8("actionRAW_data_extract_selected"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/icons/Text Document_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRAW_data_extract_selected->setIcon(icon5);
        actionRAW_data_extract_all = new QAction(MainWindow);
        actionRAW_data_extract_all->setObjectName(QString::fromUtf8("actionRAW_data_extract_all"));
        actionRAW_data_extract_all->setIcon(icon5);
        actionShow_file_information = new QAction(MainWindow);
        actionShow_file_information->setObjectName(QString::fromUtf8("actionShow_file_information"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/icons/Information_48x48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShow_file_information->setIcon(icon6);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/icons/Help_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon7);
        actionDDS_NFS = new QAction(MainWindow);
        actionDDS_NFS->setObjectName(QString::fromUtf8("actionDDS_NFS"));
        actionDDS_NFS->setEnabled(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/icons/Settings_16x16.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDDS_NFS->setIcon(icon8);
        actionBundle_assembler = new QAction(MainWindow);
        actionBundle_assembler->setObjectName(QString::fromUtf8("actionBundle_assembler"));
        actionClose_file = new QAction(MainWindow);
        actionClose_file->setObjectName(QString::fromUtf8("actionClose_file"));
        actionWrite_bundle_table = new QAction(MainWindow);
        actionWrite_bundle_table->setObjectName(QString::fromUtf8("actionWrite_bundle_table"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setFrameShape(QFrame::StyledPanel);
        splitter->setLineWidth(10);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(false);
        splitter->setHandleWidth(5);
        listWidget = new QListWidget(splitter);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMinimumSize(QSize(100, 0));
        listWidget->setMaximumSize(QSize(350, 16777215));
        splitter->addWidget(listWidget);
        frame = new QFrame(splitter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        graphicsView = new QGraphicsView(frame);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout_2->addWidget(graphicsView);

        splitter->addWidget(frame);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 989, 20));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuResource = new QMenu(menuBar);
        menuResource->setObjectName(QString::fromUtf8("menuResource"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuTools->setEnabled(true);
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuResource->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu_File->addAction(action_Open);
        menu_File->addAction(actionClose_file);
        menu_File->addSeparator();
        menu_File->addAction(actionE_xit);
        menuResource->addAction(actionExtract_selected);
        menuResource->addAction(actionExtract_all);
        menuResource->addSeparator();
        menuResource->addAction(actionRAW_data_extract_selected);
        menuResource->addAction(actionRAW_data_extract_all);
        menuResource->addSeparator();
        menuResource->addAction(actionShow_file_information);
        menuHelp->addAction(actionAbout);
        menuTools->addAction(actionDDS_NFS);
        menuTools->addAction(actionBundle_assembler);
        menuTools->addAction(actionWrite_bundle_table);
        toolBar->addAction(action_Open);
        toolBar->addSeparator();
        toolBar->addAction(actionExtract_selected);
        toolBar->addAction(actionExtract_all);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "NFS:HP Bundle Explorer", nullptr));
        action_Open->setText(QCoreApplication::translate("MainWindow", "&Open...", nullptr));
#if QT_CONFIG(tooltip)
        action_Open->setToolTip(QCoreApplication::translate("MainWindow", "Open bundle", nullptr));
#endif // QT_CONFIG(tooltip)
        actionE_xit->setText(QCoreApplication::translate("MainWindow", "E&xit", nullptr));
        actionExtract_selected->setText(QCoreApplication::translate("MainWindow", "Extract selected...", nullptr));
        actionExtract_all->setText(QCoreApplication::translate("MainWindow", "Extract all...", nullptr));
        actionRAW_data_extract_selected->setText(QCoreApplication::translate("MainWindow", "RAW data extract selected...", nullptr));
        actionRAW_data_extract_all->setText(QCoreApplication::translate("MainWindow", "RAW data extract all...", nullptr));
        actionShow_file_information->setText(QCoreApplication::translate("MainWindow", "Show file information...", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About...", nullptr));
        actionDDS_NFS->setText(QCoreApplication::translate("MainWindow", "DDS -> NFS", nullptr));
        actionBundle_assembler->setText(QCoreApplication::translate("MainWindow", "Bundle assembler...", nullptr));
        actionClose_file->setText(QCoreApplication::translate("MainWindow", "Close file", nullptr));
        actionWrite_bundle_table->setText(QCoreApplication::translate("MainWindow", "Write bundle table...", nullptr));
        menu_File->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuResource->setTitle(QCoreApplication::translate("MainWindow", "Resource", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
