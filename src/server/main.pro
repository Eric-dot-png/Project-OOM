INCLUDEPATH += ../common
CONFIG += console
TEMPLATE = app
TARGET = ./a.out
QT += core network
HEADERS += Server.h ../common/ProtocolManager.h
SOURCES += main.cpp Server.cpp ../common/ProtocolManager.cpp

       
