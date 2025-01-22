#include "client.h"

namespace oom
{
    Client::Client(int port, QObject * parent)
        : QObject(parent), socket(new QTcpSocket(this))
    {
        socket->connectToHost(QHostAddress::LocalHost,port);

        auto fn1 = [&]()
        {
            qDebug() << "Connected to the server";
            socket->write("Hello, server!");
        };
        
        connect(socket, &QTcpSocket::connected, this, fn1);

        auto fn2 = [&]()
        {
            QByteArray response = socket->readAll();
            qDebug() << "Recieved from server: " << response;
        };
        
        connect(socket, &QTcpSocket::readyRead, this, fn2);
                
    }

    Client::~Client()
    {
        delete socket;
    }
    
};
