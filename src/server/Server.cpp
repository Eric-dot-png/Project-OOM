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
                    if (db.loginValidate(u)) // if logged in
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::LoginAccept,{});
                    }
                    else // if log in failed
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
                    if (!numeric(code) || code.length() != 6) // if invalid code
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::AccountNotAuthenticated,
                            {"code not valid"} );
                    }
                    else // if valid code
                    {
                        bool success = db.emailValidate(u, code);
                        if(success) // if email validated
                        {
                            x = ProtocolManager::serialize(
                                ProtocolManager::AccountAuthenticated,
                                {} );
                        }
                        else // if email validation failed
                        {
                            x = ProtocolManager::serialize(
                                ProtocolManager::AccountNotAuthenticated,
                                {"Unknown Error, Could not authenticate account."}
                                );
                        }
                    }
                    clientSocket->write(x);
                    break;
                } // end of CreateAccountAuthCodeSubmit
                    
                case ProtocolManager::CreateAccountRequest:
                {
                    QString usr = m["Username"].toString();
                    QString pwd = m["Password"].toString();
                    QString email = m["Email"].toString();
                    User u(usr, pwd, email);
                    if (!db.availUsername(u)) // if username already used
                    {
                        x = ProtocolManager::serialize(
                            ProtocolManager::CreateAccountDenied,
                            {"username already Exists."} );
                    }
                    else // if open account credentials
                    {
                        QString code = db.newUser(u);
                        if(code != "") // if new User was created(w/o autoval)
                        {
                            std::string emailsyscall = "python3 myemail.py "
                                + u.get_email().toStdString() + ' '
                                + code.toStdString();
                            int email = std::system(emailsyscall.c_str());
                            if(email == 0) // if myemail.py has no issues
                                x = ProtocolManager::serialize(
                                    ProtocolManager::CreateAccountAccept,
                                    {usr,pwd} );
                            else // if myemail.py runs into issues
                            {
                                db.removeReg(u);
                                x = ProtocolManager::serialize(
                                    ProtocolManager::CreateAccountDenied,
                                    {"could not send email"});
                            }
                        }
                        else // if user creation failed
                        {
                            x = ProtocolManager::serialize(
                                ProtocolManager::CreateAccountDenied,
                                {"Unknown Error, Could not create account."}
                                );
                        }
                    } 
                    clientSocket->write(x);
                    break;
                } // end of CreateAccountAuthCodeRequest
                
                default:
                {
                    qDebug() << "Protocol" << m["Type"].toString()
                             << "not handled";
                }
            }
        });
    }

    //Returns 1 if all characters in s are 0 <= char <= 9
    bool Server::numeric(const QString & s) const
    {
        for(const QChar & c : s)
            if(c < '0' || c > '9')
                return 0;
        return 1;
    }
};
