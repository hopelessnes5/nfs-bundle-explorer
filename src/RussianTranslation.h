#ifndef RUSSIAN_TRANSLATION_H
#define RUSSIAN_TRANSLATION_H

#include <QTranslator>
#include <QHash>
#include <QString>

class RussianTranslator : public QTranslator
{
    Q_OBJECT
public:
    RussianTranslator(QObject* parent = nullptr) : QTranslator(parent)
    {
        // Главные меню
        m_dict["File"] = "Файл";
        m_dict["Resource"] = "Ресурсы";
        m_dict["Tools"] = "Инструменты";
        m_dict["Help"] = "Справка";
        m_dict["Language"] = "Язык";

        // Пункты меню
        m_dict["Open"] = "Открыть";
        m_dict["Close file"] = "Закрыть файл";
        m_dict["Exit"] = "Выход";
        m_dict["Extract selected"] = "Извлечь выбранное";
        m_dict["Extract all"] = "Извлечь всё";
        m_dict["RAW data extract selected"] = "Извлечь выбранное в RAW";
        m_dict["RAW data extract all"] = "Извлечь всё в RAW";
        m_dict["Show file information"] = "Свойства файла";
        m_dict["Bundle assembler"] = "Сборщик архивов";
        m_dict["DDS -> NFS"] = "Конвертер DDS -> NFS";
        m_dict["Write bundle table"] = "Экспорт таблицы бандла";
        m_dict["About"] = "О программе";

        // Контекстное меню
        m_dict["Extract resource (%1) ..."] = "Извлечение ресурсов (%1) ...";
        m_dict["Show file information..."] = "Свойства файла...";
        m_dict["View defs table..."] = "Просмотр таблицы определений...";
        m_dict["Save strings..."] = "Сохранить строки...";

        // Типы ресурсов
        m_dict["texture"] = "текстура";
        m_dict["material"] = "материал";
        m_dict["model"] = "3D-модель";
        m_dict["defs table"] = "таблица определений";
        m_dict["strings"] = "локализация";
        m_dict["unknown"] = "неизвестно";

        // Диалоги
        m_dict["Select NFS:HP bundle file"] = "Выберите файл архива NFS:HP";
        m_dict["NFS:HP bundle (*.bndl *.bundle *.bin)"] = "Архивы NFS:HP (*.bndl *.bundle *.bin)";
        m_dict["Where to save NFS:HP texture?"] = "Куда сохранить текстуру?";
        m_dict["DDS Images (*.dds)"] = "Изображения DDS (*.dds)";
        m_dict["Where to save NFS:HP model?"] = "Куда сохранить 3D-модель?";
        m_dict["OBJ Model (*.obj)"] = "3D-модель Wavefront (*.obj)";
        m_dict["Where to save RAW file?"] = "Куда сохранить RAW-файл?";
        m_dict["RAW file (*.raw)"] = "RAW файлы (*.raw)";
        m_dict["NFS RAW Files (*.nfs)"] = "Файлы NFS (*.nfs)";
        m_dict["Where to save files?"] = "Выберите папку для извлечения всех файлов:";
        m_dict["Where to save RAW files?"] = "Выберите папку для извлечения RAW файлов:";
        m_dict["Where to save TXT file?"] = "Куда сохранить TXT файл?";
        m_dict["TXT Files (*.txt)"] = "Текстовые файлы (*.txt)";
        m_dict["Select DDS texture"] = "Выберите текстуру DDS";
        m_dict["Where to save table info?"] = "Куда сохранить таблицу бандла?";

        m_dict["Warning"] = "Предупреждение";
        m_dict["Error"] = "Ошибка";
        m_dict["Error!"] = "Ошибка!";
        m_dict["Success"] = "Успешно";
        m_dict["No files selected"] = "Файлы не выбраны";
        m_dict["Could not save texture!"] = "Не удалось сохранить текстуру!";
        m_dict["Could not save model!"] = "Не удалось сохранить модель!";
        m_dict["Could not save file!"] = "Не удалось сохранить файл!";
        m_dict["Could not write file!"] = "Не удалось записать файл!";
        m_dict["Some files could not be saved"] = "Некоторые файлы не удалось сохранить";
        m_dict["File saved"] = "Файл успешно сохранён";
        m_dict["Files saved"] = "Все файлы успешно сохранены";
        m_dict["Strings saved"] = "Строки успешно сохранены";
        m_dict["Texture converted"] = "Текстура успешно сконвертирована";
        m_dict["Texture not converted"] = "Не удалось сконвертировать текстуру";
    }

    // КРИТИЧЕСКИ ВАЖНО: говорим Qt, что транслятор НЕ пустой!
    bool isEmpty() const override
    {
        return false;
    }

    QString translate(const char* context, const char* sourceText, const char* disambiguation = nullptr, int n = -1) const override
    {
        Q_UNUSED(context);
        Q_UNUSED(disambiguation);
        Q_UNUSED(n);

        if (!sourceText) return QString();

        QString key = QString::fromUtf8(sourceText).trimmed();
        if (m_dict.contains(key)) return m_dict.value(key);

        QString cleanKey = key;
        cleanKey.remove('&');
        bool hasDots = cleanKey.endsWith(QStringLiteral("..."));
        if (hasDots) cleanKey.chop(3);

        if (m_dict.contains(cleanKey)) {
            QString res = m_dict.value(cleanKey);
            if (hasDots) res += QStringLiteral("...");
            return res;
        }

        return QString();
    }

private:
    QHash<QString, QString> m_dict;
};

#endif // RUSSIAN_TRANSLATION_H