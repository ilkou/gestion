SOURCES += \
	source/client.cpp \
    source/dbmanager.cpp \
    source/editaccount.cpp \
    source/employe.cpp \
    source/espaceclient.cpp \
    source/espaceemploye.cpp \
    source/firstwin.cpp \
    source/main.cpp \
    source/newaccount.cpp \
    source/newcar.cpp \
    source/newemploye.cpp \
    source/reserver.cpp

HEADERS += \
    include/client.h \
    include/dbmanager.h \
    include/editaccount.h \
    include/employe.h \
    include/espaceclient.h \
    include/espaceemploye.h \
    include/firstwin.h \
    include/includes.h \
    include/newaccount.h \
    include/newcar.h \
    include/newemploye.h \
    include/reserver.h

INCLUDEPATH += include

OBJECTS_DIR = generated_files

MOC_DIR = generated_files

DESTDIR = app

QT += widgets sql core

RESOURCES += mesressources.qrc

CONFIG += release
