// file : server.cpp
// name : eric garcia

#include "server.h"

namespace oom
{
    Server::Server(int port, QObject * parent)
        : QObject(parent), port_(port), listener_(new QTcpServer(this))
    {
        connect(listener_, &QTcpServer::newConnection, this,
                &Server::onNewConnection);
        // CHANGE LocalHost to any
        if (listener_->listen(QHostAddress::LocalHost, port))
            qDebug() << "SUCCESS: Server is listening on port "
                     << port << "...";
        else
            qDebug() << "ERROR: Server could not start!";
    }

    
    Server::~Server()
    {
        delete listener_;
    }

    
    void Server::onNewConnection()
    {
        QTcpSocket * clientSocket = listener_->nextPendingConnection();

        auto fn = [clientSocket]()
        {
            QByteArray data = clientSocket->readAll();
            qDebug() << "Recieved from client:" << data;
            clientSocket->write("Recieved");
        };
        connect(clientSocket, &QTcpSocket::readyRead, this, fn);
    }
};
