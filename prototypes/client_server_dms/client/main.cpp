// file : main.cpp
// name : eric garcia

#include <QCoreApplication>
#include <QHostAddress>
#include <iostream>

#include "Client.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    oom::Client client;
    QHostAddress host = QHostAddress::LocalHost;
    int port = 1234;
    
    client.connectToServer(host,port);
    client.login("eric","123");
    
    return app.exec();
}
