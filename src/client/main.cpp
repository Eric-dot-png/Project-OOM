//Names: Eric Garcia
//       Bethany Spatafora
//       Alex Franke
//File: main.cpp
//OOM project

#include "login.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login loginWindow;
    MainWindow mainWindow;

    loginWindow.resize(800, 600);
    loginWindow.show();

    QObject::connect(&loginWindow, &Login::loginSuccessful, [&]() {
        QSize loginSize = loginWindow.size();
        mainWindow.resize(loginSize);
        loginWindow.close();
        mainWindow.show();
    });

    return a.exec();
}
