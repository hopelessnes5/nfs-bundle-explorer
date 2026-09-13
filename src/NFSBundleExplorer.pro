#-------------------------------------------------
#
# Project created by QtCreator 2010-11-21T22:16:15
#
#-------------------------------------------------

QT       += core gui widgets

# Включаем современный стандарт C++17
CONFIG   += c++17

# Заставляем компилятор MSVC правильно понимать UTF-8
win32-msvc* {
    QMAKE_CXXFLAGS += /utf-8
}

TARGET = NFSBundleExplorer
TEMPLATE = app

INCLUDEPATH += libs/zlib

SOURCES += main.cpp\
        mainwindow.cpp \
    NFSTexture.cpp \
    DDS.cpp \
    QtMappedFile.cpp \
    libs/zlib/zutil.c \
    libs/zlib/uncompr.c \
    libs/zlib/trees.c \
    libs/zlib/inftrees.c \
    libs/zlib/inflate.c \
    libs/zlib/inffast.c \
    libs/zlib/infback.c \
    libs/zlib/gzwrite.c \
    libs/zlib/gzread.c \
    libs/zlib/gzlib.c \
    libs/zlib/gzclose.c \
    libs/zlib/deflate.c \
    libs/zlib/crc32.c \
    libs/zlib/compress.c \
    libs/zlib/adler32.c \
    zlib_adapter.cpp \
    dialogs/dlgfileinfo.cpp \
    NFSModel.cpp \
    dialogs/DlgBundleAssembler.cpp \
    NFSBundle.cpp \
    dialogs/dlgdefstableview.cpp \
    NFSStringsTable.cpp

HEADERS += mainwindow.h \
    RussianTranslation.h \
    NFSTexture.h \
    DDS.h \
    common.h \
    QtMappedFile.h \
    libs/zlib/zutil.h \
    libs/zlib/zlib.h \
    libs/zlib/zconf.h \
    libs/zlib/trees.h \
    libs/zlib/inftrees.h \
    libs/zlib/inflate.h \
    libs/zlib/inffixed.h \
    libs/zlib/inffast.h \
    libs/zlib/gzguts.h \
    libs/zlib/deflate.h \
    libs/zlib/crc32.h \
    zlib_adapter.h \
    NFSStructuresDef.h \
    NFSTypes.h \
    dialogs/dlgfileinfo.h \
    NFSModel.h \
    dialogs/DlgBundleAssembler.h \
    NFSBundle.h \
    NFSMath.h \
    dialogs/dlgdefstableview.h \
    NFSStringsTable.h

FORMS    += mainwindow.ui \
    dialogs/dlgfileinfo.ui \
    dialogs/DlgBundleAssembler.ui \
    dialogs/dlgdefstableview.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc

RC_FILE = NFSBundleExplorer.rc
