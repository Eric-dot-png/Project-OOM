// file : main.cpp
// name : eric garcia

#include <QCoreApplication>
#include <QHostAddress>

#include "client.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    QHostAddress host = QHostAddress::LocalHost;
    int port = 1234;
    
    oom::Client client;
    client.connectToServer(host,port);

    
    return app.exec();
}
