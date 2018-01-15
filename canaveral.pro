######################################################################
# Automatically generated by qmake (3.0) Wed Jun 14 16:25:47 2017
######################################################################

TEMPLATE = app
TARGET = canaveral
INCLUDEPATH += .
DESTDIR = bin
OBJECTS_DIR = .obj
MOC_DIR = .moc
QT += widgets
QT += core
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += jack

# Input
HEADERS += src/backend.h \
           src/buttons_tab.h \
           src/constants.h \
           src/frontend.h \
           src/pad.h \
           src/samples_tab.h \
           src/volume_tab.h \
           src/select_window.h \
           src/sample.h \
           src/wav_parser.h \
           src/hotkey_config.h \
           src/preset_window.h
SOURCES += src/backend.cpp \
           src/buttons_tab.cpp \
           src/frontend.cpp \
           src/main.cpp \
           src/pad.cpp \
           src/samples_tab.cpp \
           src/volume_tab.cpp \
           src/select_window.cpp \
           src/sample.cpp \
           src/wav_parser.cpp \
           src/hotkey_config.cpp \
           src/preset_window.cpp
