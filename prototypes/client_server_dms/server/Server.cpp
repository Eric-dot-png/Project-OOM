// file : server.cpp
// name : eric garcia

#include "Server.h"

namespace oom
{
    Server::Server(int port, QObject * parent)
        : QObject(parent), port_(port), listener_(new QTcpServer(this))
    {
        connect(listener_, &QTcpServer::newConnection, this,
                &Server::onNewConnection);

        if (listener_->listen(QHostAddress::LocalHost, port))
            qDebug() << "Listening on port" << port << "...";
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
        connect(clientSocket, &QTcpSocket::readyRead, this, [clientSocket]()
        {
            QString data = clientSocket->readAll();
            qDebug() << "Recieved" << data << "from" << clientSocket;
            
            ProtocolManager::MessageType t = ProtocolManager::classify(data);
            QString msg = ProtocolManager::contents(data);
            
            switch(t)
            {
                case ProtocolManager::LoginRequest:
                {
                    QStringList ms = msg.split(' ');
                    QString usr = ms[0];
                    QString pwd = ms[1];
                    
                    QByteArray x = ProtocolManager::constructMsg(
                        ProtocolManager::LoginAccept,{});
                    clientSocket->write(x);
                    break;
                }
                default:
                {
                    qDebug() << "Protocol" << t << "not handled";
                }
            }
        });
    } 
};
