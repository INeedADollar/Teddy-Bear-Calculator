QT += core gui


greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

CONFIG += c++1z
TEMPLATE = app
RC_ICONS = icons/calculator-icon.ico
VERSION = 1.0.0.0
QMAKE_TARGET_DESCRIPTION = "Calculator"
TARGET = "calculator"

LIBS += -lwinmm

SOURCES += \
    main.cpp

RESOURCES += \
    icons.qrc

HEADERS +=
