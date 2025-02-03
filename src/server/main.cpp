// file : main.cpp

#include <iostream>
#include <QtCore>

#include "Server.h"
#include "dbHandler.h"
#include <mysql.h>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    int port = 1234; // port for server to listen on
    oom::Server s(port);
    
    return app.exec();
    
    return 0;
}

