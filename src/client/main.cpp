//Names: Eric Garcia
//       Bethany Spatafora
//       Alex Franke
//File: main.cpp
//OOM project

#include "applicationhandler.h"
#include "Client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QLoggingCategory::setFilterRules("*.debug=true");

    QApplication a(argc, argv);
    oom::Client *c = new oom::Client;
    ApplicationHandler oomApp(c);
    oomApp.resize(800, 600);
    oomApp.show();

    int out = a.exec();
    delete c;
    
    return out;
}
