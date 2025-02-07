INCLUDEPATH += ../common
TEMPLATE = app

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

SOURCES += \
    Client.cpp \
    ../common/ProtocolManager.cpp \
    applicationhandler.cpp \
    authenticationcode.cpp \
    main.cpp \
    login.cpp \
    privatemessages.cpp \
    register.cpp
    
HEADERS += \
    Client.h \
    ../common/ProtocolManager.h \
    ../common/User.h \
    applicationhandler.h \
    authenticationcode.h \
    login.h \
    privatemessages.h \
    register.h

FORMS += \
    authenticationcode.ui \
    login.ui \
    privatemessages.ui \
    register.ui

TARGET = ./a.out
    
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
