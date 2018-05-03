QT += core gui

TARGET = blockgenerator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    blockinfo.cpp

DISTFILES +=

RESOURCES += \
    resources.qrc

HEADERS += \
    blockinfo.h

unix:!maxc {
    DEFINES += "POINT_SIZE=60"
    DEFINES += "TITLE_MAX_LEN=21"
    DEFINES += "DEFAULT_POINT_SIZE=50"
    DEFINES += "FONT_WIDTH=32"
}
win32 {
    DEFINES += "POINT_SIZE=54"
    DEFINES += "TITLE_MAX_LEN=18"
    DEFINES += "DEFAULT_POINT_SIZE=42"
    DEFINES += "FONT_WIDTH=34"
}
