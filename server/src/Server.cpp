// file : server.cpp
// name : eric garcia

#include "Server.h"

// Password encrytion basically
// make sure the password is encrypted EVERY TIME a User object is created
QString pwdHash(const QString & s)
{
    QString newS = s;
    for(int i = 0; i < 10; i++)
        newS = QString::number(qHash(newS));
    return newS;
}

Server * Server::instance(NULL);

Server::Server(int port, QObject * parent)
    : QObject(parent), port_(port), listener_(new QTcpServer(this))
{
    connect(listener_, &QTcpServer::newConnection, this,
            &Server::onNewConnection);
        
    if (listener_->listen(QHostAddress::LocalHost, port))
        qDebug() << "Listening on port" 
                 << listener_->serverPort() << "...";
    else
        qDebug() << "ERROR: Server could not start!";
    db = dbHandler::GetInstance();
}
    
Server::~Server()
{
    listener_->close();
}
    
Server * Server::getInstance()
{
    if (instance == NULL)
        instance = new Server(1234);
    return instance;
}
    
void Server::destroyInstance()
{
    if (instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}
    
void Server::onNewConnection()
{
    QTcpSocket * clientSocket = listener_->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, [this,
                                                         clientSocket]()
    {
        // lambda function to handle client requests
        QByteArray data = clientSocket->readAll();
        qDebug() << "Recieved" << data;
            
        QJsonObject m = ProtocolManager::deserialize(data);

        QByteArray x; // message to send back
        switch(m["Type"].toInt())
        {
            case ProtocolManager::PrivateMessageForward:
            {
                qDebug() << "recieved msg forward...";
                // put the message into the logger.
                    
                break;
            }
            case ProtocolManager::PrivateMessageRequest:
            {
                qDebug() << "recieved msg request...";
                QString uname = m["To"].toString();
                QString msg = m["Message"].toString();

                User u(uname,"","");
                bool exists = !db->availUsername(u);
                auto p = usermap.find(uname);
                if (p == usermap.end() && !exists)
                {
                    x = ProtocolManager::serialize(
                        ProtocolManager::PrivateMessageDenied,
                        {"User Does Not Exist. "}
                        );
                }
                else if (p == usermap.end())
                {
                    qDebug() << "User is not online... logging msg";
                    // log message here
                    x = ProtocolManager::serialize(
                        ProtocolManager::PrivateMessageDenied,
                        {"User is not online."}
                        );
                }
                else
                {
                    qDebug() << "User is online, logging msg";
                    // log message here
                    auto ip_port = p->second;
                    x = ProtocolManager::serialize(
                        ProtocolManager::PrivateMessageAccept,
                        {uname,ip_port.first.toString(),
                         QString::number(ip_port.second),msg}
                        );
                }
                clientSocket->write(x);
                break;
            }
            case ProtocolManager::LoginRequest:
            {
                QString usr = m["Username"].toString();
                QString pwd = m["Password"].toString();
                pwd = pwdHash(pwd);
                User u(usr, pwd);
                if (db->loginValidate(u)) // if logged in
                {
                    x = ProtocolManager::serialize(
                        ProtocolManager::LoginAccept,{usr});
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
                pwd = pwdHash(pwd);
                User u(usr, pwd);
                if (!numeric(code) || code.length() != 6) // if invalid code
                {
                    x = ProtocolManager::serialize(
                        ProtocolManager::AccountNotAuthenticated,
                        {"code not valid"} );
                }
                else // if valid code
                {
                    bool success = db->emailValidate(u, code);
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
                pwd = pwdHash(pwd);
                User u(usr, pwd, email);
                if (!db->availUsername(u)) // if username already used
                {
                    x = ProtocolManager::serialize(
                        ProtocolManager::CreateAccountDenied,
                        {"username already Exists."} );
                }
                else // if open account credentials
                {
                    QString code = db->newUser(u);
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
                            db->removeReg(u);
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
            case ProtocolManager::AnnounceIpPort:
            {
                QString usr = m["Username"].toString();
                QHostAddress ip(m["Ip"].toString());
                quint16 port = m["Port"].toString().toInt();
                if (usermap.find(usr) != usermap.end())
                {
                    qDebug() << "Warning! User already in map."
                             << "Overwriting...";
                }
                usermap[usr] = {ip,port};
                    
                qDebug() << usr << "->" << ip << "," << port;
                break;
            }

            case ProtocolManager::AnnounceOffline:
            {
                QString usr = m["Username"].toString();

                usermap.erase(usr);
                    
                qDebug() << usr << "has been removed from usrmap";
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
    
//Returns 1 if all characters in s are 0 <= char <= 9
bool Server::numeric(const QString & s) const
{
    for(const QChar & c : s)
        if(c < '0' || c > '9')
            return 0;
    return 1;
}
