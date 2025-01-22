#include "client.h"

namespace oom
{
    Client::Client(int port, QObject * parent)
        : QObject(parent), socket(new QTcpSocket(this)),
          stdinNotifier(new QSocketNotifier(fileno(stdin),
                                            QSocketNotifier::Read, this))
    {
        socket->connectToHost(QHostAddress::LocalHost,port);
        
        auto fn1 = [&]()
        {
            qDebug() << "CLIENT: Connected to the server";
        };
        connect(socket, &QTcpSocket::connected, this, fn1);

        auto fn2 = [&]()
        {
            QByteArray response = socket->readAll();
            qDebug() << "SERVER: " << response;
        };
        connect(socket, &QTcpSocket::readyRead, this, fn2);

        connect(stdinNotifier, &QSocketNotifier::activated, this,
                &Client::readInput);
    }
    
    void Client::readInput()
    {
        QTextStream stdinStream(stdin);
        QByteArray input = stdinStream.readLine().toUtf8();
        
        socket->write(input); // send input to the server
    }
                        
    Client::~Client()
    {
        delete socket;
    }
    
};
