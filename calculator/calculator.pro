QT += core gui


greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

CONFIG += c++1z
TEMPLATE = app
RC_ICONS = icons/calculator-icon.ico
VERSION = 1.0.0.0
QMAKE_TARGET_DESCRIPTION = "Calculator"
TARGET = "cp"

LIBS += -lwinmm

HEADERS += \
    calculator.h \
    imagerenderer.h \
    mainwindow.h \
    processthread.h \
    settingswindow.h \
    soundthread.h

SOURCES += \
    calculator.cpp \
    imagerenderer.cpp \
    main.cpp \
    mainwindow.cpp \
    processthread.cpp \
    settingswindow.cpp \
    soundthread.cpp

RESOURCES += \
    icons.qrc


