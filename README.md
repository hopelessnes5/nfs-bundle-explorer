# nfs-bundle-explorer
iOrange NFS:HP Bundle Explorer

<img width="991" height="725" alt="nfs-bundle-explorer" src="https://github.com/user-attachments/assets/e6284592-899f-4a50-bc32-05a4e495099f" />

NFS:HP Bundle Explorer v0.3 (Modernized Edition)

EN

Global update and modernization of the utility for exploring and building archives
 (*.bndl / *.bundle) for the games Need for Speed: Hot Pursuit (2010) and Burnout Paradise.

The original 2010 project (author: iOrange) was completely redesigned, freed from 
dependencies dating back 15 years, migrated to a modern technology stack, 
and received full localization into Russian.

🚀 Key changes and improvements:

1. Migration to a modern stack (Qt 5/6 and C++17)

  - Support for modern OS and compilers: The project was migrated from the old Qt 4 
    and Visual Studio 2008 to Qt 5.15 / Qt 6 and MSVC 2019 / 2022 compilers.
  - C++17 standard: The code was completely cleaned of outdated constructs 
    (the register class was removed, outdated macros were replaced with static_assert, 
	constexpr and <cstdint> were introduced).
  - Memory safety (RAII): Manual memory management (new/delete) was replaced with modern 
	smart pointers std::unique_ptr and STL containers, eliminating resource leaks.

2. Fixing critical errors of the original

  - Crash fixes: Fixed the program crash with the Access Violation (0xC0000005) error 
	when previewing textures in the interface window.
  - Memory leak fixes: Eliminated the constant memory leak in the zlib archiver adapter 
	that occurred each time compressed data was read.
  - File reading bug fix: Fixed a critical error in QtMappedFile, which caused 
	byte-by-byte reading to always return the first byte of the file.
  - Correct archive assembly: Fixed an error in the bundle builder (PrepareTextureFile), 
	where the lack of a return value led to packaging failures on release optimizations.
  - Binary compatibility: Strict byte-by-byte packing of structures (#pragma pack) was 
	introduced, ensuring data integrity on 32- and 64-bit systems.

3. New features and convenience

  - 🌐 Russian language support: Full Russian localization was added (interface, 
	context menus, file types, tips, and error messages).
  - Instant language switching: A Language / Язык item was added to the main menu to switch 
	between English and Russian «on the fly» without restarting the program.
  - Support for paths with Cyrillic (Unicode): Saving and converting textures/models was migrated 
	to work through QFile, thanks to which the program correctly works with folders in Russian.

Authors:

Original version (2010): iOrange
Modernization, bug fixes, and localization (2026): @hopelessnes5 & Gemini

RU

Глобальное обновление и модернизация утилиты для исследования и сборки архивов
(*.bndl / *.bundle) игр Need for Speed: Hot Pursuit (2010) и Burnout Paradise.

Оригинальный проект 2010 года (автор: iOrange) был полностью переработан,
избавлен от зависимостей 15-летней давности, переведён на современный стек
технологий и получил полноценную русификацию.

🚀 Основные изменения и улучшения:

1. Переход на современный стек (Qt 5/6 и C++17)

  - Поддержка современных ОС и компиляторов: Проект переведен со старого Qt 4 и
    Visual Studio 2008 на Qt 5.15 / Qt 6 и компиляторы MSVC 2019 / 2022.
  - Стандарт C++17: Код полностью очищен от устаревших конструкций (убран
    ключевой класс register, устаревшие макросы заменены на стандартный
    static_assert, внедрены constexpr и <cstdint>).
  - Безопасность памяти (RAII): Ручное управление памятью (new/delete) заменено
    на современные умные указатели std::unique_ptr и контейнеры STL, исключая
    утечки ресурсов.

2. Исправление критических ошибок оригинала

  - Устранение вылетов (Crash Fix): Исправлено падение программы с ошибкой
    Access Violation (0xC0000005) при предпросмотре текстур в окне интерфейса.
  - Исправление утечек памяти (Memory Leaks): Ликвидирована постоянная утечка
    памяти в адаптере архиватора zlib, возникавшая при каждом чтении сжатых
    данных.
  - Исправление бага чтения файлов: Устранена критическая ошибка в QtMappedFile,
    из-за которой побайтовое чтение всегда возвращало первый байт файла.
  - Корректная сборка архивов: Исправлена ошибка в сборщике бандлов
    (PrepareTextureFile), где отсутствовал возврат значения, приводивший к сбоям
    упаковки на релизных оптимизациях.
  - Бинарная совместимость: Внедрена строгая побайтовая упаковка структур
    (#pragma pack), гарантирующая целостность данных на 32- и 64-битных
    системах.

3. Новые возможности и удобство

  - 🌐 Поддержка русского языка: Добавлена полноценная русская локализация
    (интерфейс, контекстные меню, типы файлов, подсказки и сообщения об
    ошибках).
  - Мгновенное переключение языка: В главное меню добавлен пункт Language / Язык
    для переключения между English и Русский «на лету» без перезапуска
    программы.
  - Поддержка путей с кириллицей (Unicode): Сохранение и конвертация
    текстур/моделей переведены на работу через QFile, благодаря чему программа
    корректно работает с папками на русском языке.

Авторы:

  - Оригинальная версия (2010): iOrange
  - Модернизация, исправление ошибок и локализация (2026): @hopelessnes5 &
    Gemini
