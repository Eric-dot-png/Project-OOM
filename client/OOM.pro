INCLUDEPATH += ../common \
			   includes

TEMPLATE = app

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

BUILD_DIR = $$PWD/build

QMAKE_MKDIR = mkdir -p
QMAKE_CLEAN += $$BUILD_DIR
clean.target = clean_build_dir
clean.commands = system(rm -rf $$BUILD_DIR)
QMAKE_EXTRA_TARGETS += clean

DESTDIR = $$BUILD_DIR/bin      # binary files
MOC_DIR = $$BUILD_DIR/moc      # moc_* files
UI_DIR = $$BUILD_DIR/ui        # ui_* files
OBJECTS_DIR = $$BUILD_DIR/obj  # .o files

run.target = run
run.commands = ./build/bin/a.out
QMAKE_EXTRA_TARGETS += run
          
SOURCES += \
    src/oomwidget.cpp \
    src/Client.cpp \
    ../common/ProtocolManager.cpp \
    src/applicationhandler.cpp \
    src/authenticationcode.cpp \
    src/main.cpp \
    src/login.cpp \
    src/message.cpp \
    src/privatemessages.cpp \
    src/register.cpp
    
HEADERS += \
    includes/oomwidget.h \
    includes/Client.h \
    ../common/ProtocolManager.h \
    ../common/User.h \
    includes/applicationhandler.h \
    includes/authenticationcode.h \
    includes/login.h \
    includes/message.h \
    includes/privatemessages.h \
    includes/register.h

FORMS += \
    forms/authenticationcode.ui \
    forms/login.ui \
    forms/privatemessages.ui \
    forms/register.ui

TARGET = ./a.out
    
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
