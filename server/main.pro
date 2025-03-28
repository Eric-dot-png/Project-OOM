INCLUDEPATH += ../common/includes /usr/include/mysql -m64 includes
CONFIG += console
TEMPLATE = app
TARGET = ./a.out
QT += core network

SOURCES += $$files(src/*.cpp, true) \
           $$files(../common/src/*.cpp, true)

HEADERS += $$files(includes/*.h, true) \
           $$files(../common/includes/*.h, true)
           
LIBS += -L/usr/lib64/mysql -lmysqlclient -lz -lzstd -lssl -lcrypto -lresolv -lm

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
