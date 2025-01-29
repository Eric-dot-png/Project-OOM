// file : main.cpp
// name : eric garcia

#include <iostream>

#include <QCoreApplication>

#include "Server.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);
    
    oom::Server myserver(1234);
    
    return app.exec();
}
