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
        connect(clientSocket, &QTcpSocket::readyRead, this, [this,
                                                             clientSocket]()
        {
            QString data = clientSocket->readAll();
            qDebug() << "Recieved" << data << "from" << clientSocket;
            
            ProtocolManager::Protocol t = ProtocolManager::classify(data);
            QString msg = ProtocolManager::contents(data);
            
            switch(t)
            {
                case ProtocolManager::LoginRequest:
                {
                    QStringList ms = msg.split(' ');
                    QString usr = ms[0];
                    QString pwd = ms[1];

                    QByteArray x;
                    if (dbContains(usr,pwd))
                    {
                        x = ProtocolManager::constructMsg(
                            ProtocolManager::LoginAccept,{});
                    }
                    else
                    {
                        x = ProtocolManager::constructMsg(
                            ProtocolManager::LoginDenied, {});
                    }
                    clientSocket->write(x);
                    break;
                }
                case ProtocolManager::CreateAccountRequest:
                {
                    QStringList ms = msg.split(' ');
                    QString usr = ms[0];
                    QString pwd = ms[1];
                    QString email = ms[1];

                    QByteArray x;
                    if (dbContains(usr,pwd) || !valid(usr,pwd))
                    {
                        x = ProtocolManager::constructMsg(
                            ProtocolManager::CreateAccountDenied, {}
                            );
                    }
                    else
                    {
                        createAccount(usr,pwd,email);
                        x = ProtocolManager::constructMsg(
                            ProtocolManager::CreateAccountAccept, {usr,pwd}
                            );
                    }
                    clientSocket->write(x);
                    break;
                }
                default:
                {
                    qDebug() << "Protocol" << t << "not handled";
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
    
    bool Server::dbContains(const QString& usr, const QString& pwd) const
    {
        std::string path = "db.txt";
        std::ifstream database(path);
        
        if (!database)
        {
            qDebug() << "Database could not be opened!?!?!";
            return false;
        }
        else
        {
            std::string line;
            while (std::getline(database,line))
            {
                QString str = QString::fromStdString(line);
                QStringList ss = str.split(' ');
                if (ss[0] == usr && ss[1] == pwd)
                    return true;
            }
            return false;
        }
        database.close();
        
    }

    void Server::createAccount(const QString& usr, const QString& pwd,
                               const QString& email)
    {
        std::string path = "db.txt";
        std::ofstream database(path, std::ios::app);
        database << usr.toStdString() << ' ' << pwd.toStdString()
                 << std::endl;
        database.close();
    }
};
