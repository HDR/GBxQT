TEMPLATE = app
TARGET = GBxQT
DEPENDPATH += .
INCLUDEPATH += .

QT += network widgets serialport winextras

QMAKE_LFLAGS += -static

RESOURCES += qdarkstyle/style.qrc

# Input
HEADERS += \
           src/Console.h \
           src/const.h \
           src/Gui.h \
           src/Settings.h \
           src/icon.xpm
SOURCES += src/gbcflsh.cpp \
           src/Gui.cpp \
           src/Settings.cpp
RC_FILE = src/res.rc

DISTFILES += \
    src/icon.xpm
