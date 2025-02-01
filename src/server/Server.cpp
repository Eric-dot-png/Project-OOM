// file : server.cpp
// name : eric garcia

#include "Server.h"
#include "dbHandler.h"

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
        connect(clientSocket, &QTcpSocket::readyRead, this, [this,
                                                             clientSocket]()
        {
            QByteArray data = clientSocket->readAll();
            qDebug() << "Recieved" << data << "from" << clientSocket;

            QJsonObject m = ProtocolManager::deserialize(data);

            QByteArray x;
            switch(m["Type"].toInt())
            {
                case ProtocolManager::LoginRequest:
                {
                    QString usr = m["Username"].toString();
                    QString pwd = m["Password"].toString();
                    User u(usr, pwd);
                    if (db.loginValidate(u))
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::LoginAccept,{});
                    }
                    else
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::LoginDenied, {});
                    }
                    clientSocket->write(x);
                    break;
                }
                case ProtocolManager::CreateAccountRequest:
                {
                    QString usr = m["Username"].toString();
                    QString pwd = m["Password"].toString();
                    QString email = m["Email"].toString();
                    User u(usr, pwd, email);
                    if (!db.availUsername(u) || !valid(usr,pwd))
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::CreateAccountDenied, {}
                            );
                    }
                    else
                    {
                        bool success = newUser(u);
                        if(success)
                            x = ProtocolManager::serialize(
                                ProtocolManager::CreateAccountAccept,
                                {usr,pwd}
                                );
                        else
                            // Hi Beth here, what do if can't create User for
                            // some reason?
                    }
                    clientSocket->write(x);
                    break;
                }
                default:
                {
                    qDebug() << "Protocol" << m["Type"].toString()
                             << "not handled";
                }
            }
        });
    } // end of fn

    bool Server::valid(const QString& usr, const QString& pwd) const
    {
        auto f = [](const QString& s) {
            for (const QChar& c : s)
                if ( !( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                        || (c >= '1' && c <= '9')) )
                    return false;
            return true && s.size() != 0;
        };
        return f(usr) && f(pwd);
    }
};
