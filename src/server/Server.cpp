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
                            ProtocolManager::LoginDenied,
                            {"account with login information does not exist"}
                            );
                    }
                    clientSocket->write(x);
                    break;
                } // end of LoginRequest
                case ProtocolManager::CreateAccountAuthCodeSubmit:
                {
                    QString usr = m["Username"].toString();
                    QString pwd = m["Password"].toString();
                    QString code = m["Code"].toString();
                    User u(usr, pwd);
                    if (!numeric(code) || code.length() != 6)
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::AccountNotAuthenticated,
                            {"code not valid"} );
                    }
                    else
                    {
                        bool success = db.emailValidate(u, code);
                        if(success)
                        {
                            x = ProtocolManager::serialize(
                                ProtocolManager::AccountAuthenticated,
                                {} );
                        }
                        else
                        {
                            x = ProtocolManager::serialize(
                                ProtocolManager::AccountNotAuthenticated,
                                {"Unknown Error, Could not authenticate account."}
                                );
                        }
                    } // end of CreateAccountRequest
                    clientSocket->write(x);
                    break;
                }
                case ProtocolManager::CreateAccountRequest:
                {
                    QString usr = m["Username"].toString();
                    QString pwd = m["Password"].toString();
                    QString email = m["Email"].toString();
                    User u(usr, pwd, email);
                    if (!db.availUsername(u)) 
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::CreateAccountDenied,
                            {"username already Exists."} );
                    }
                    else if (!valid(usr,pwd))
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::CreateAccountDenied,
                            {"username or password not valid"} );
                    }
                    else
                    {
                        QString code = db.newUser(u);
                        if(code != "")
                        {
                            std::string emailsyscall = "python3 myemail.py "
                                + u.get_email().toStdString() + ' '
                                + code.toStdString();
                            int email = std::system(emailsyscall.c_str());
                            if(email == 0)
                                x = ProtocolManager::serialize(
                                    ProtocolManager::CreateAccountAccept,
                                    {usr,pwd} );
                            else
                            {
                                db.removeReg(u);
                                x = ProtocolManager::serialize(
                                    ProtocolManager::CreateAccountDenied,
                                    {"could not send email"});
                            }
                        }
                        else
                        {
                            x = ProtocolManager::serialize(
                                ProtocolManager::CreateAccountDenied,
                                {"Unknown Error, Could not create account."}
                                );
                        }
                    } // end of CreateAccountAuthCodeRequest
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
    }

    bool Server::valid(const QString& usr, const QString& pwd) const
    {
        auto f = [](const QString& s) {
            for (const QChar& c : s)
                if ( !( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                        || (c >= '1' && c <= '9')) )
                    return false;
            return true && s.size() != 0;
        };
        //return f(usr) && f(pwd);
        return true;
    }

    bool Server::numeric(const QString & s) const
    {
        for(const QChar & c : s)
            if(c < '0' || c > '9')
                return 0;
        return 1;
    }
};
