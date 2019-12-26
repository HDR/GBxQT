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
           src/Device.h \
           src/const.h \
           src/Gui.h \
           src/Settings.h \
           src/icon.xpm \
           src/rs232/rs232.h
SOURCES += \
           src/Device.cpp \
           src/GBxQT.cpp \
           src/Gui.cpp \
           src/Settings.cpp \
           src/rs232/rs232.c
RC_FILE = src/res.rc

DISTFILES += \
    src/icon.xpm
