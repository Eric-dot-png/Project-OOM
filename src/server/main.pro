INCLUDEPATH += ../common /usr/include/mysql -m64
CONFIG += console
TEMPLATE = app
TARGET = ./a.out
QT += core network
HEADERS += Server.h dbHandler.h ../common/ProtocolManager.h
SOURCES += main.cpp Server.cpp dbHandler.cpp  ../common/ProtocolManager.cpp
LIBS += -L/usr/lib64/mysql -lmysqlclient -lz -lzstd -lssl -lcrypto -lresolv -lm
