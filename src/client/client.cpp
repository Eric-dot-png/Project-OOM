#include "client.h"

namespace oom
{
    Client::Client(QObject * parent)
        : QObject(parent), socket(new QTcpSocket(this)),
          inNotif(new QSocketNotifier(fileno(stdin),
                                      QSocketNotifier::Read,this))
    {
        auto onConnect = [&]()
        {
            qDebug() << "CLIENT: Connected to the server.";
        };
        connect(socket, &QTcpSocket::connected, this, onConnect);
        
        auto onDisconnect = [&]()
        {
            qDebug() << "CLIENT: Disconnected from the server.";
        };
        connect(socket, &QTcpSocket::disconnected, this, onDisconnect);
        
        connect(socket, &QTcpSocket::readyRead,this,&Client::onServerReply);
        
        connect(inNotif, &QSocketNotifier::activated, this, &Client::sendMsg);
    }
    
    Client::~Client()
    {
        delete socket;
    }
    
    void Client::connectToServer(const QHostAddress& host, int port)
    {
        socket->connectToHost(host,port);
    }

    void Client::sendMsg()
    {
        QTextStream stdinStream(stdin);
        QByteArray input = stdinStream.readLine().toUtf8();
        
        socket->write(input); // send input to the server
    }
    
    void Client::onServerReply()
    {
        QString reply(socket->readAll());
        qDebug() << "SERVER: " << reply;
        
    }
    
};
