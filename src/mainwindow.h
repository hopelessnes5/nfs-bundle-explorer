#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QAction>
#include <memory>

#include "common.h"
#include "NFSTexture.h"
#include "NFSModel.h"
#include "NFSBundle.h"
#include "RussianTranslation.h"

namespace Ui {
    class MainWindow;
}

class CNFSTexture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    [[nodiscard]] int           GetSelectedFileIndex();
    [[nodiscard]] CNFSTexture*  GetTextureFromFileIndex(int idx);
    [[nodiscard]] CNFSModel*    GetModelFromFileIndex(int idx);
    int                         SaveRAWFile(int idx, const QString& fileName);
    void                        OpenBundle(const QString& fileName);
    void                        UpdateFileList();
    void                        SetLanguage(int index);

protected:
    void changeEvent(QEvent* event) override;

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<CNFSBundle>     m_Bundle;
    QGraphicsScene*                 m_GraphScene = nullptr;
    RussianTranslator               m_ruTranslator;
    QAction*                        m_actEnglish = nullptr;
    QAction*                        m_actRussian = nullptr;
    QMenu*                          m_langMenu   = nullptr;

private slots:
    void on_actionClose_file_triggered();
    void on_actionBundle_assembler_triggered();
    void on_actionDDS_NFS_triggered();
    void on_actionAbout_triggered();
    void on_actionShow_file_information_triggered();
    void on_actionView_defs_table_triggered();
    void on_actionSave_strings_triggered();
    void on_actionRAW_data_extract_all_triggered();
    void on_actionRAW_data_extract_selected_triggered();
    void on_actionExtract_all_triggered();
    void on_actionExtract_selected_triggered();
    void on_action_Open_triggered();
    void on_actionE_xit_triggered();
    void onFileSelected();
    void onListViewContextMenu(const QPoint& pos);
    void on_actionWrite_bundle_table_triggered();
};

#endif // MAINWINDOW_H