INCLUDEPATH += ../common
CONFIG += console
TEMPLATE = app
TARGET = ./a.out
QT += core network
HEADERS += Client.h ../common/ProtocolManager.h
SOURCES += main.cpp Client.cpp ../common/ProtocolManager.cpp
