INCLUDEPATH += ../common
TEMPLATE = app

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 + console

SOURCES += \
    Client.cpp \
    ../common/ProtocolManager.cpp \
    baseuser.cpp \
    devuser.cpp \
    main.cpp \
    login.cpp \
    privatechat.cpp \
    profile.cpp \
    register.cpp \
    user.cpp
    
HEADERS += \
    Client.h \
    ../common/ProtocolManager.h \
    baseuser.h \
    devuser.h \
    login.h \
    privatechat.h \
    profile.h \
    register.h \
    user.h

FORMS += \
    login.ui \
    privatechat.ui \
    profile.ui \
    register.ui

TARGET = ./a.out
    
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
