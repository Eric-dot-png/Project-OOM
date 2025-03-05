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

//Returns 1 if all characters in s are 0 <= char <= 9
bool numeric(const QString & s) 
{
    for(const QChar & c : s)
        if(c < '0' || c > '9')
            return 0;
    return 1;
}

Server * Server::instance(NULL);

Server::Server(QObject * parent)
    : QObject(parent), listener(new QTcpServer(this)),
      timer(new QTimer(this))
{
    timer->setInterval(60000); 
    connect(listener, &QTcpServer::newConnection, this,
            &Server::onNewConnection);

    connect(timer, &QTimer::timeout, this, &Server::update);
    
    if (listener->listen(SERVER_HOST_IP, SERVER_PORT))
        qDebug() << "Listening on port" 
                 << listener->serverPort() << "...";
    else
        qDebug() << "ERROR: Server could not start!";
    db = dbHandler::GetInstance();

    timer->start();
}
    
Server::~Server()
{
    listener->close();
}
    
Server * Server::getInstance()
{
    if (instance == NULL)
        instance = new Server;
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

void Server::update()
{
    db->cleanReg();
    timer->start();
}

void Server::onNewConnection()
{
    QTcpSocket * clientSocket = listener->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, [this,
                                                         clientSocket]()
    {
        // lambda function to handle client requests
        QByteArray data = clientSocket->readAll();
        qDebug() << "Recieved" << data << "from"
                 << clientSocket->socketDescriptor();
        
        QJsonObject m = ProtocolManager::deserialize(data);

        Protocol type = static_cast<Protocol>(m["Type"].toInt());
        
        QByteArray x; // message to send back
        switch(type)
        {
            case Protocol::PrivateMessage:
            {
                qDebug() << "Recieved private message";
                QString to = m["To"].toString();
                db->storeMessage(m);
                
                if (onlineUserMap.find(to) != onlineUserMap.end())
                    onlineUserMap[to]->write(data);
                break;
            }
            case Protocol::DiscoveryRequest:
            {
                qDebug() << "recieved discovery request...";
                QString usrname = m["Username"].toString();
                bool existing_user = db->userExists(usrname);
                if (existing_user)
                {
                    QString messagehistory = db->getMessages(m["CurrUser"].toString(), usrname);
                    qDebug() << "\t" << messagehistory;
                    x = ProtocolManager::serialize(
                        Protocol::DiscoveryAccept, {usrname, messagehistory}
                        );
                }
                else
                    x = ProtocolManager::serialize( Protocol::DiscoveryFail,
                                                    {usrname} );
                clientSocket->write(x);
                break;
            }
            case Protocol::LoginRequest:
            {
                QString usr = m["Username"].toString();
                QString pwd = m["Password"].toString();
                pwd = pwdHash(pwd);
                if (db->loginValidate(usr, pwd)) // if logged in
                {
                    x = ProtocolManager::serialize(
                        Protocol::LoginAccept,{usr});
                    onlineUserMap[usr] = clientSocket;
                }
                else // if log in failed
                {
                    x = ProtocolManager::serialize(
                        Protocol::LoginDenied,
                        {"account with login information does not exist"}
                        );
                }
                clientSocket->write(x);
                break;
            } // end of LoginRequest
                
            case Protocol::AuthCodeSubmit:
            {
                QString usr = m["Username"].toString();
                QString code = m["Code"].toString();
                if (!numeric(code) || code.length() != 6) // if invalid code
                {
                    x = ProtocolManager::serialize(
                        Protocol::AuthCodeDenied,
                        {"code not valid"} );
                }
                else // if valid code
                {
                    bool success = db->emailValidate(usr, code);
                    if(success) // if email validated
                    {
                        x = ProtocolManager::serialize(
                            Protocol::AuthCodeAccept, {} );
                    }
                    else // if email validation failed
                    {
                        x = ProtocolManager::serialize(
                            Protocol::AuthCodeDenied,
                            {"Unknown Error, Could not authenticate account."}
                            );
                    }
                }
                clientSocket->write(x);
                break;
            } 
            case Protocol::CreateAccountRequest:
            {
                QString usr = m["Username"].toString();
                QString pwd = m["Password"].toString();
                QString email = m["Email"].toString();
                pwd = pwdHash(pwd);
                if (!db->availUsername(usr)) // if username already used
                {
                    x = ProtocolManager::serialize(
                        Protocol::CreateAccountDenied,
                        {"username already Exists."} );
                }
                else // if open account credentials
                {
                    QString code = db->newUser(usr, pwd, email);
                    if(code != "") // if new User was created(w/o autoval)
                    {
                        std::string emailsyscall = "python3 src/myemail.py "
                            + email.toStdString() + ' '
                            + code.toStdString();
                        int email = std::system(emailsyscall.c_str());
                        if(email == 0) // if myemail.py has no issues
                            x = ProtocolManager::serialize(
                                Protocol::CreateAccountAccept,
                                {usr,pwd} );
                        else // if myemail.py runs into issues
                        {
                            db->removeReg(usr);
                            x = ProtocolManager::serialize(
                                Protocol::CreateAccountDenied,
                                {"could not send email"});
                        }
                    }
                    else // if user creation failed
                    {
                        x = ProtocolManager::serialize(
                            Protocol::CreateAccountDenied,
                            {"Unknown Error, Could not create account."}
                            );
                    }
                } 
                clientSocket->write(x);
                break;
            }
            default:
            {
                qDebug() << "Unknown request... ";
                break;
            }
        }
    });
}


