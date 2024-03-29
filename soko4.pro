######################################################################
# Automatically generated by qmake (2.01a) ?? 20. ??? 11:27:03 2012
######################################################################

CONFIG += qt \
  warn_on \
  console \
  debug

QT += widgets gui

TEMPLATE = app
TARGET = soko4
DEPENDPATH += src
INCLUDEPATH += src

DESTDIR     += bin
OBJECTS_DIR += tmp
MOC_DIR     += build
UI_DIR      += build

# Input
HEADERS += src/cboard.h \
           src/ccell.h \
           src/clevel.h \
           src/clevelset.h \
           src/cmove.h \
           src/cpixmap.h \
           src/cpixmaps.h \
           src/cplayboard.h \
           src/cplayfield.h \
           src/crelateddirs.h \
           src/csokoban.h \
           src/ctheme.h \
           src/mainwindow.h \
           src/textwindow.h
FORMS += src/mainwindow.ui src/textwindow.ui
SOURCES += src/cboard.cpp \
           src/ccell.cpp \
           src/clevel.cpp \
           src/clevelset.cpp \
           src/cmove.cpp \
           src/cpixmap.cpp \
           src/cpixmaps.cpp \
           src/cplayboard.cpp \
           src/cplayfield.cpp \
           src/crelateddirs.cpp \
           src/csokoban.cpp \
           src/ctheme.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/textwindow.cpp
