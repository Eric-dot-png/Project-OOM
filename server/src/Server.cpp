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
    QTcpSocket * client = listener->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, [this,
                                                         client]()
    {
        // lambda function to handle client requests
        QByteArray data = client->readAll();
        qDebug() << "Recieved" << data << "from"
                 << client->socketDescriptor();
        
        QJsonObject m = ProtocolManager::deserialize(data);
        
        Protocol type = static_cast<Protocol>(m["Type"].toInt());

        switch(type)
        {
            case Protocol::FriendRequest:
            {
                //qDebug() << m["From"].toString() << ' ' << m["To"].toString();
                bool succ = db->addFriendRequest(m["From"].toString(),
                                                 m["To"].toString());
                writeToUserRaw(m["To"].toString(), data);
                break;
            }
            case Protocol::FriendAccept:
            {
                //qDebug() << m["From"].toString() << ' ' << m["To"].toString();
                bool succ = db->addFriend(m["From"].toString(), m["To"].toString());
                writeToUserRaw(m["To"].toString(), data);
                break;
            }
            case Protocol::FriendRemoved:
            {
                break;
            }
            case Protocol::PrivateMessage:
            {
                db->storeMessage(m);
                writeToUserRaw(m["To"].toString(),data);
                break;
            }
            case Protocol::DiscoveryRequest:
            {
                QString usrname = m["Username"].toString();
                bool existing_user = db->userExists(usrname);
                if (existing_user)
                {
                    QString msgHist =db->getMessages(m["CurrUser"].toString(),
                                                     usrname);
                    writeToSocket(client, Protocol::DiscoveryAccept,
                                  {usrname, msgHist});
                }
                else
                {
                    writeToSocket(client, Protocol::DiscoveryFail, {usrname});
                }
                break;
            }
            case Protocol::LoginRequest:
            {
                QString usr = m["Username"].toString();
                QString pwd = m["Password"].toString();
                pwd = pwdHash(pwd);
                if (db->loginValidate(usr, pwd)) // if logged in
                {
                    writeToSocket(client, Protocol::LoginAccept, {usr});
                    onlineUserMap[usr] = client;
                }
                else // if log in failed
                {
                    writeToSocket(client, Protocol::LoginDenied,
                                  {"Account with login information DNE"});
                }
                break;
            } 
            case Protocol::AuthCodeSubmit:
            {
                QString usr = m["Username"].toString();
                QString code = m["Code"].toString();
                if (!numeric(code) || code.length() != 6) // if invalid code
                {
                    writeToSocket(client, Protocol::AuthCodeDenied,
                                  {"Invalid Code"});
                }
                else // if valid code
                {
                    bool success = db->emailValidate(usr, code);
                    if(success) // if email validated
                    {
                        writeToSocket(client,Protocol::AuthCodeAccept, {});
                    }
                    else // if email validation failed
                    {
                        writeToSocket(client, Protocol::AuthCodeDenied,
                          {"Unknown Error, Could not authenticate account."});
                    }
                }
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
                    writeToSocket(client, Protocol::CreateAccountDenied,
                                  {"Username already exists."});
                }
                else // if open account credentials
                {
                    QString code = db->newUser(usr, pwd, email);
                    if(code != "") // if new User was created(w/o autoval)
                    {
                        std::string emailsyscall = "python3 src/myemail.py "
                            + email.toStdString() + ' ' + code.toStdString();
                        int email = std::system(emailsyscall.c_str());
                        if(email == 0) // if myemail.py has no issues
                        {
                            writeToSocket(client,
                                          Protocol::CreateAccountAccept,
                                          {usr, pwd});
                        }
                        else // if myemail.py runs into issues
                        {
                            db->removeReg(usr);
                            writeToSocket(client,
                                          Protocol::CreateAccountDenied,
                                          {"Could not send email."});
                        }
                    }
                    else // if user creation failed
                    {
                        writeToSocket(client, Protocol::CreateAccountDenied,
                               {"Unknown Error, Could not create account."}); 
                    }
                } 
                break;
            }
            case Protocol::FriendList:
            {
                //qDebug() << "Protocol::FriendList";
                QString usr = m["From"].toString();
                QStringList list = db->getFriendslist(usr);
                for (const QString &s : list)
                {
                    qDebug() << s << '\n';
                }
                QJsonArray jarray;
                for (const QString &str : list)
                    jarray.append(str);
                QJsonValue j = jarray;
                if (!jarray.isEmpty())
                {
                    writeToSocket(client, Protocol::FriendListAccept, {usr, j});
                }
                else
                {
                    writeToSocket(client, Protocol::FriendListFailed, {usr, j});
                }

                break;
            }
            case Protocol::FriendRequestList:
            {
                //qDebug() << "Protocol::FriendRequestList";
                QString usr = m["From"].toString();
                QStringList list = db->getFriendRequests(usr);
                for (const QString &s : list)
                {
                    qDebug() << s << '\n';
                }
                QJsonArray jarray;
                for (const QString &str : list)
                    jarray.append(str);
                QJsonValue j = jarray;
                if (!jarray.isEmpty())
                {
                    writeToSocket(client, Protocol::FriendRequestListAccept, {usr, j});
                }
                else
                {
                    writeToSocket(client, Protocol::FriendRequestListFailed, {usr, j});
                }

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

void Server::writeToUserRaw(const QString& to, const QByteArray& data)
{
    if (onlineUserMap.find(to) != onlineUserMap.end() &&
        onlineUserMap.find(to)->second != NULL)
    {
        onlineUserMap[to]->write(data);
    }
}

void Server::writeToSocket(QTcpSocket * socket, Protocol type,
                           const QList<QJsonValue>& argv)
{
    if (socket != NULL)
    {
        socket->write(ProtocolManager::serialize(type,argv));
    }
}
