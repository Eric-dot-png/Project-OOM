INCLUDEPATH += ../common
CONFIG += console
TEMPLATE = app
TARGET = ./a.out
QT += core network sql
HEADERS += Server.h dbHandler.h ../common/ProtocolManager.h
SOURCES += main.cpp Server.cpp dbHandler.cpp  ../common/ProtocolManager.cpp

       
