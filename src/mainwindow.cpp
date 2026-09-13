#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>
#include <QMimeData>
#include <QDir>
#include <QMenu>
#include <QMenuBar>
#include <QActionGroup>
#include <QCoreApplication>

#include "dialogs/dlgfileinfo.h"
#include "dialogs/DlgBundleAssembler.h"
#include "dialogs/dlgdefstableview.h"

#include "NFSStringsTable.h"
#include "zlib_adapter.h"

QString GetFileTypeString(uint32 fileType)
{
    switch (fileType)
    {
        case NFS_FILE_TEXTURE:       return qApp->translate("MainWindow", "texture");
        case NFS_FILE_MATERIAL:      return qApp->translate("MainWindow", "material");
        case NFS_FILE_MODEL:         return qApp->translate("MainWindow", "model");
        case NFS_FILE_DEFS_TABLE:    return qApp->translate("MainWindow", "defs table");
        case NFS_FILE_STRINGS_TABLE: return qApp->translate("MainWindow", "strings");
        default:                     return qApp->translate("MainWindow", "unknown");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_Bundle(nullptr)
    , m_GraphScene(nullptr)
{
    ui->setupUi(this);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QWidget::customContextMenuRequested, this, &MainWindow::onListViewContextMenu);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onFileSelected);

    // Создаем меню выбора языка прямо в строке меню окна
    m_langMenu = menuBar()->addMenu(tr("Language"));

    auto* langGroup = new QActionGroup(this);

    m_actEnglish = m_langMenu->addAction(QStringLiteral("English"));
    m_actEnglish->setCheckable(true);
    m_actEnglish->setChecked(true);
    langGroup->addAction(m_actEnglish);

    m_actRussian = m_langMenu->addAction(QStringLiteral("Русский"));
    m_actRussian->setCheckable(true);
    langGroup->addAction(m_actRussian);

    connect(m_actEnglish, &QAction::triggered, this, [this]() { SetLanguage(0); });
    connect(m_actRussian, &QAction::triggered, this, [this]() { SetLanguage(1); });

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete m_GraphScene;
}

void MainWindow::SetLanguage(int index)
{
    if (index == 1) {
        qApp->installTranslator(&m_ruTranslator);
    } else {
        qApp->removeTranslator(&m_ruTranslator);
    }

    // Принудительно вызываем перерисовку интерфейса
    ui->retranslateUi(this);

    if (m_langMenu) {
        m_langMenu->setTitle(index == 1 ? QStringLiteral("Язык") : QStringLiteral("Language"));
    }

    // Прямой перевод заголовков меню верхнего уровня на случай несовпадения в .ui
    for (QAction* action : menuBar()->actions()) {
        if (QMenu* menu = action->menu()) {
            if (menu == m_langMenu) continue;
            QString text = action->text();
            text.remove('&');
            if (index == 1) {
                if (text == "File") action->setText(QStringLiteral("Файл"));
                else if (text == "Resource") action->setText(QStringLiteral("Ресурс"));
                else if (text == "Tools") action->setText(QStringLiteral("Инструменты"));
                else if (text == "Help") action->setText(QStringLiteral("Справка"));
            } else {
                if (text == "Файл") action->setText(QStringLiteral("File"));
                else if (text == "Ресурс") action->setText(QStringLiteral("Resource"));
                else if (text == "Инструменты") action->setText(QStringLiteral("Tools"));
                else if (text == "Справка") action->setText(QStringLiteral("Help"));
            }
        }
    }

    UpdateFileList();
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        if (m_langMenu) {
            m_langMenu->setTitle(tr("Language"));
        }
        UpdateFileList(); // Мгновенно обновляем список файлов на русском
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::UpdateFileList()
{
    if (!m_Bundle) return;

    const int count = m_Bundle->GetFilesCount();
    for (int i = 0; i < count; ++i)
    {
        const S_NFSFileInfo& info = m_Bundle->GetFileInfo(i);
        QString title = QStringLiteral("file_%1 (%2) [%3 KB]")
                            .arg(i, 4, 10, QChar('0'))
                            .arg(GetFileTypeString(info.fileType))
                            .arg(static_cast<float>(info.hdrUncompSize + info.dataUncompSize) / 1024.0f, 0, 'f', 1);

        if (auto* item = ui->listWidget->item(i)) {
            item->setText(title);
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QString filePath = event->mimeData()->urls().first().toLocalFile();
        if (filePath.length() > 3) {
            event->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    if (!event->mimeData()->hasUrls()) {
        event->ignore();
        return;
    }

    QString filePath = event->mimeData()->urls().first().toLocalFile();
    OpenBundle(filePath);
    event->acceptProposedAction();
}

int MainWindow::GetSelectedFileIndex()
{
    const auto selected = ui->listWidget->selectedItems();
    if (selected.isEmpty()) return -1;

    QVariant var = selected.first()->data(Qt::UserRole);
    bool ok = false;
    int idx = var.toInt(&ok);
    return ok ? idx : -1;
}

CNFSTexture* MainWindow::GetTextureFromFileIndex(int idx)
{
    if (!m_Bundle) return nullptr;

    const S_NFSFileInfo& info = m_Bundle->GetFileInfo(idx);
    if (info.fileType != NFS_FILE_TEXTURE) return nullptr;

    auto* compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileHeaderOffset));
    std::vector<uint8> headerData(info.hdrUncompSize, 0);

    if (zlib_adapter::DecompressMemory(compData, info.hdrCompSize, headerData))
    {
        compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileBodyOffset));
        std::vector<uint8> imageData(info.dataUncompSize, 0);

        if (zlib_adapter::DecompressMemory(compData, info.dataCompSize, imageData))
        {
            auto* tex = new CNFSTexture();
            tex->InitFromData(headerData, imageData);
            return tex;
        }
    }
    return nullptr;
}

CNFSModel* MainWindow::GetModelFromFileIndex(int idx)
{
    if (!m_Bundle) return nullptr;

    const S_NFSFileInfo& info = m_Bundle->GetFileInfo(idx);
    if (info.fileType != NFS_FILE_MODEL) return nullptr;

    auto* compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileHeaderOffset));
    std::vector<uint8> headerData(info.hdrUncompSize, 0);

    if (zlib_adapter::DecompressMemory(compData, info.hdrCompSize, headerData))
    {
        compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileBodyOffset));
        std::vector<uint8> modelData(info.dataUncompSize, 0);

        if (zlib_adapter::DecompressMemory(compData, info.dataCompSize, modelData))
        {
            auto* mdl = new CNFSModel();
            mdl->InitFromData(headerData, modelData);
            return mdl;
        }
    }
    return nullptr;
}

int MainWindow::SaveRAWFile(int idx, const QString& fileName)
{
    if (!m_Bundle) return 0;

    const S_NFSFileInfo& info = m_Bundle->GetFileInfo(idx);
    auto* compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileHeaderOffset));

    std::vector<uint8> headerData(info.hdrUncompSize, 0);
    if (zlib_adapter::DecompressMemory(compData, info.hdrCompSize, headerData))
    {
        std::vector<uint8> fileData;
        if (info.dataCompSize > 0)
        {
            compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileBodyOffset));
            fileData.resize(info.dataUncompSize, 0);
            if (!zlib_adapter::DecompressMemory(compData, info.dataCompSize, fileData)) {
                return 0;
            }
        }

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) return 0;

        file.write(reinterpret_cast<const char*>(headerData.data()), headerData.size());
        if (!fileData.empty()) {
            file.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
        }
        file.close();
        return 1;
    }
    return 0;
}

void MainWindow::OpenBundle(const QString& fileName)
{
    m_Bundle = std::make_unique<CNFSBundle>();
    m_Bundle->InitFromFile(fileName);

    ui->listWidget->clear();
    const int count = m_Bundle->GetFilesCount();

    for (int i = 0; i < count; ++i)
    {
        const S_NFSFileInfo& info = m_Bundle->GetFileInfo(i);

        QString title = QStringLiteral("file_%1 (%2) [%3 KB]")
                            .arg(i, 4, 10, QChar('0'))
                            .arg(GetFileTypeString(info.fileType))
                            .arg(static_cast<float>(info.hdrUncompSize + info.dataUncompSize) / 1024.0f, 0, 'f', 1);

        auto* item = new QListWidgetItem(title);

        if (info.fileType == NFS_FILE_TEXTURE)
            item->setIcon(QIcon(":/resources/icons/Picture_16x16.png"));
        else if (info.fileType == NFS_FILE_MODEL)
            item->setIcon(QIcon(":/resources/icons/Blue SRT-4.png"));
        else
            item->setIcon(QIcon(":/resources/icons/New_16x16.png"));

        item->setData(Qt::UserRole, QVariant(i));
        ui->listWidget->addItem(item);
    }
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::onFileSelected()
{
    delete m_GraphScene;
    m_GraphScene = nullptr;

    int idx = GetSelectedFileIndex();
    if (idx < 0) return;

    std::unique_ptr<CNFSTexture> texture(GetTextureFromFileIndex(idx));
    if (texture)
    {
        const int width      = texture->GetWidth();
        const int height     = texture->GetHeight();
        const int components = texture->GetNumComponents();

        if (width <= 0 || height <= 0 || (components != 3 && components != 4)) {
            return;
        }

        const size_t bufferSize = static_cast<size_t>(width) * height * components;
        std::vector<uint8> rgbImage(bufferSize, 0);

        if (texture->Decompress(rgbImage.data()) > 0)
        {
            m_GraphScene = new QGraphicsScene(this);
            QImage::Format fmt = (components == 3) ? QImage::Format_RGB888 : QImage::Format_ARGB32;

            QImage img = QImage(rgbImage.data(), width, height, width * components, fmt).copy();

            if (!img.isNull())
            {
                m_GraphScene->addPixmap(QPixmap::fromImage(img));
                ui->graphicsView->resetCachedContent();
                ui->graphicsView->setScene(m_GraphScene);
                ui->graphicsView->show();
            }
        }
    }
}

void MainWindow::onListViewContextMenu(const QPoint& pos)
{
    int idx = GetSelectedFileIndex();
    if (idx < 0 || !m_Bundle) return;

    const S_NFSFileInfo& info = m_Bundle->GetFileInfo(idx);

    QMenu menu(ui->listWidget);
    menu.addAction(QIcon(":/resources/icons/Save_24x24.png"),
                   tr("Extract resource (%1) ...").arg(GetFileTypeString(info.fileType)),
                   this, &MainWindow::on_actionExtract_selected_triggered);
    menu.addSeparator();
    menu.addAction(QIcon(":/resources/icons/Information_48x48.png"),
                   tr("Show file information..."),
                   this, &MainWindow::on_actionShow_file_information_triggered);

    if (NFS_FILE_DEFS_TABLE == info.fileType)
    {
        menu.addSeparator();
        menu.addAction(tr("View defs table..."), this, &MainWindow::on_actionView_defs_table_triggered);
    }
    else if (NFS_FILE_STRINGS_TABLE == info.fileType)
    {
        menu.addSeparator();
        menu.addAction(tr("Save strings..."), this, &MainWindow::on_actionSave_strings_triggered);
    }

    menu.exec(ui->listWidget->mapToGlobal(pos));
}

void MainWindow::on_action_Open_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Select NFS:HP bundle file"), QString(), tr("NFS:HP bundle (*.bndl *.bundle *.bin)"));
    if (name.length() < 3) return;

    OpenBundle(name);
}

void MainWindow::on_actionClose_file_triggered()
{
    ui->listWidget->clear();
    m_Bundle.reset();
}

void MainWindow::on_actionExtract_selected_triggered()
{
    int idx = GetSelectedFileIndex();
    if (idx < 0 || !m_Bundle)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No files selected"), QMessageBox::Ok);
        return;
    }

    bool fail = true;
    const S_NFSFileInfo& info = m_Bundle->GetFileInfo(idx);

    if (info.fileType == NFS_FILE_TEXTURE)
    {
        QString name = QFileDialog::getSaveFileName(this, tr("Where to save NFS:HP texture?"), QString(), tr("DDS Images (*.dds)"));
        if (name.length() < 3) return;

        std::unique_ptr<CNFSTexture> texture(GetTextureFromFileIndex(idx));
        if (!texture || !texture->SaveAsDDS(name)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not save texture!"), QMessageBox::Ok);
        } else {
            fail = false;
        }
    }
    else if (info.fileType == NFS_FILE_MODEL)
    {
        QString name = QFileDialog::getSaveFileName(this, tr("Where to save NFS:HP model?"), QString(), tr("OBJ Model (*.obj)"));
        if (name.length() < 3) return;

        std::unique_ptr<CNFSModel> model(GetModelFromFileIndex(idx));
        if (!model || !model->SaveAsOBJ(name)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not save model!"), QMessageBox::Ok);
        } else {
            fail = false;
        }
    }
    else
    {
        QString name = QFileDialog::getSaveFileName(this, tr("Where to save RAW file?"), QString(), tr("RAW file (*.raw)"));
        if (name.length() < 3) return;

        if (!SaveRAWFile(idx, name)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not save file!"), QMessageBox::Ok);
        } else {
            fail = false;
        }
    }

    if (!fail) {
        QMessageBox::information(this, tr("Success"), tr("File saved"), QMessageBox::Ok);
    }
}

void MainWindow::on_actionExtract_all_triggered()
{
    if (!m_Bundle) return;

    QString dir = QFileDialog::getExistingDirectory(this, tr("Where to save files?"));
    if (dir.length() < 3) return;

    dir = QDir::cleanPath(dir) + '/';
    bool someErrors = false;

    const int count = m_Bundle->GetFilesCount();
    for (int i = 0; i < count; ++i)
    {
        const auto& info = m_Bundle->GetFileInfo(i);
        if (info.fileType == NFS_FILE_TEXTURE)
        {
            std::unique_ptr<CNFSTexture> texture(GetTextureFromFileIndex(i));
            if (texture) {
                QString name = dir + QStringLiteral("file_%1.dds").arg(i, 4, 10, QChar('0'));
                if (!texture->SaveAsDDS(name)) someErrors = true;
            } else {
                someErrors = true;
            }
        }
        else if (info.fileType == NFS_FILE_MODEL)
        {
            std::unique_ptr<CNFSModel> model(GetModelFromFileIndex(i));
            if (model) {
                QString name = dir + QStringLiteral("file_%1.obj").arg(i, 4, 10, QChar('0'));
                if (!model->SaveAsOBJ(name)) someErrors = true;
            } else {
                someErrors = true;
            }
        }
        else
        {
            QString name = dir + QStringLiteral("file_type_%1_%2.raw").arg(info.fileType).arg(i, 4, 10, QChar('0'));
            if (!SaveRAWFile(i, name)) {
                someErrors = true;
            }
        }
    }

    if (someErrors)
        QMessageBox::warning(this, tr("Error"), tr("Some files could not be saved"), QMessageBox::Ok);
    else
        QMessageBox::information(this, tr("Success"), tr("Files saved"), QMessageBox::Ok);
}

void MainWindow::on_actionRAW_data_extract_selected_triggered()
{
    int idx = GetSelectedFileIndex();
    if (idx < 0) {
        QMessageBox::warning(this, tr("Warning"), tr("No files selected"), QMessageBox::Ok);
        return;
    }

    QString name = QFileDialog::getSaveFileName(this, tr("Where to save RAW file?"), QString(), tr("NFS RAW Files (*.nfs)"));
    if (name.length() < 3) return;

    if (!SaveRAWFile(idx, name))
        QMessageBox::critical(this, tr("Error"), tr("Could not save file!"), QMessageBox::Ok);
    else
        QMessageBox::information(this, tr("Success"), tr("File saved"), QMessageBox::Ok);
}

void MainWindow::on_actionRAW_data_extract_all_triggered()
{
    if (!m_Bundle) return;

    QString dir = QFileDialog::getExistingDirectory(this, tr("Where to save RAW files?"));
    if (dir.length() < 3) return;

    dir = QDir::cleanPath(dir) + '/';
    bool someErrors = false;

    const int count = m_Bundle->GetFilesCount();
    for (int i = 0; i < count; ++i)
    {
        QString name = dir + QStringLiteral("file_%1.nfs").arg(i, 4, 10, QChar('0'));
        if (!SaveRAWFile(i, name)) someErrors = true;
    }

    if (someErrors)
        QMessageBox::warning(this, tr("Error"), tr("Some files could not be saved"), QMessageBox::Ok);
    else
        QMessageBox::information(this, tr("Success"), tr("Files saved"), QMessageBox::Ok);
}

void MainWindow::on_actionShow_file_information_triggered()
{
    int idx = GetSelectedFileIndex();
    if (idx >= 0 && m_Bundle)
    {
        DlgFileInfo dfi(this);
        dfi.SetFileInfo(m_Bundle->GetFileInfo(idx), m_Bundle.get());
        dfi.exec();
    }
}

void MainWindow::on_actionView_defs_table_triggered()
{
    int idx = GetSelectedFileIndex();
    if (idx >= 0 && m_Bundle)
    {
        DlgDefsTableView dlg(this);
        dlg.SetBundleAndIdx(m_Bundle.get(), idx);
        dlg.exec();
    }
}

void MainWindow::on_actionSave_strings_triggered()
{
    int idx = GetSelectedFileIndex();
    if (idx >= 0 && m_Bundle)
    {
        QString name = QFileDialog::getSaveFileName(this, tr("Where to save TXT file?"), QString(), tr("TXT Files (*.txt)"));
        if (name.length() < 3) return;

        const S_NFSFileInfo& info = m_Bundle->GetFileInfo(idx);
        auto* compData = reinterpret_cast<uint8*>(m_Bundle->GetDataAtOffset(info.fileHeaderOffset));

        std::vector<uint8> headerData(info.hdrUncompSize, 0);
        zlib_adapter::DecompressMemory(compData, info.hdrCompSize, headerData);

        CNFSStringsTable strTable;
        if (strTable.InitFromData(headerData))
        {
            if (strTable.SaveAsTXT(name)) {
                QMessageBox::information(this, tr("Success"), tr("Strings saved"), QMessageBox::Ok);
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Could not write file!"), QMessageBox::Ok);
            }
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, tr("About..."),
                             tr("Need For Speed: Hot Pursuit Bundle Explorer v0.3\n"
                                "Written by iOrange, 2010\n"
                                "Modernized for C++17 & Qt 5/6 by Gemini 3.8 Flash & @hopelessnes5 (2026)"),
                             QMessageBox::Ok);
}

void MainWindow::on_actionDDS_NFS_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Select DDS texture"), QString(), tr("DDS Images (*.dds)"));
    if (name.length() < 3) return;

    if (CNFSTexture::ConvertDDS2NFS(name))
        QMessageBox::information(this, tr("Success"), tr("Texture converted"), QMessageBox::Ok);
    else
        QMessageBox::critical(this, tr("Error"), tr("Texture not converted"), QMessageBox::Ok);
}

void MainWindow::on_actionBundle_assembler_triggered()
{
    DlgBundleAssembler bundleAssembler(this);
    bundleAssembler.exec();
}

void MainWindow::on_actionWrite_bundle_table_triggered()
{
    if (!m_Bundle) return;

    QString name = QFileDialog::getSaveFileName(this, tr("Where to save table info?"), QString(), tr("TXT Files (*.txt)"));
    if (name.length() < 3) return;

    QFile file(name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        const int count = m_Bundle->GetFilesCount();
        for (int i = 0; i < count; ++i)
        {
            const S_NFSTableEntry& fileEntry = m_Bundle->GetFileEntry(i);
            QString line = QStringLiteral("entry: hash = %1, linkID = %2, fileType = %3, numLinks = %4\n")
                               .arg(fileEntry.hash)
                               .arg(fileEntry.linkID)
                               .arg(fileEntry.fileType)
                               .arg(fileEntry.numLinks);
            file.write(line.toUtf8());
        }
        file.close();
    }
}