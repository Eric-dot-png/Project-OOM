INCLUDEPATH += ../common /usr/include/mysql -m64 \
				includes
CONFIG += console
TEMPLATE = app
TARGET = ./a.out
QT += core network
HEADERS += includes/Server.h includes/dbHandler.h ../common/ProtocolManager.h
SOURCES += src/main.cpp src/Server.cpp src/dbHandler.cpp  ../common/ProtocolManager.cpp
LIBS += -L/usr/lib64/mysql -lmysqlclient -lz -lzstd -lssl -lcrypto -lresolv -lm
