// file : Client.cpp
// name : eric garcia

#include "Client.h"
#include "regMachine.h"

Client * Client::instance(NULL);
    
Client::Client(QObject * parent)
    : QObject(parent), state(ClientState::Disconnected),
      socket(new QTcpSocket(this)),
      current_user("None","None","None"), // for now this is guest user
      listener(NULL)
{
    connect(socket, &QTcpSocket::connected, this, [&](){
        qDebug() << "Connected to Server";
        state = ClientState::Connected;
        emit connectedToServer();
    });
    
    connect(socket, &QTcpSocket::disconnected, this, [&](){
        qDebug() << "Disconnected from Server";
        state = ClientState::Disconnected;
        emit disconnectedFromServer();
    });
        
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReply);
}
    
Client::~Client()
{
    if (state != ClientState::Disconnected)
        disconnect();
        
    delete socket;
}

Client * Client::getInstance()
{
    if (instance == NULL)
        instance = new Client();
    return instance;
}
    
void Client::destroyInstance()
{
    if (instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}
    
void Client::connectToServer(const QHostAddress& host, int port)
{
    if (state == ClientState::Disconnected)
    {
        qDebug() << "Attempting to connect to server...";
        socket->connectToHost(host,port);
        state = ClientState::Connecting;
    }
    else
    {
        qDebug() << "Already connected to server.";
    }
}
    
void Client::disconnect()
{
    if (state != ClientState::Disconnected)
    {
        qDebug() << "Disconnecting from server...";
        if (state == ClientState::LoggedIn)
        {
            closeListener();
        }
            
        socket->disconnectFromHost();
        state = Disconnecting;
    }
    else
    {
        qDebug() << "Already disconnected.";
    }
}
    
void Client::writeToServer(ProtocolManager::MessageType t,
                           const QStringList& argv)
{
    socket->write(ProtocolManager::serialize(t,argv));
}

void Client::login(const User & u)
{
    if (state == ClientState::Connected)
    {
        qDebug() << "Attempting Login...";
        socket->write(ProtocolManager::serialize(
                          ProtocolManager::LoginRequest,
                          {u.get_username(), u.get_password()})
            );
        state = ClientState::LoggingIn;
    }
    else
    {
        qDebug() << "Can't Login. Current State:" << state;
    }
}
    
void Client::createAccount(const User & u)
{
    if (state == ClientState::Connected)
    {
        RegMachine::getInstance()->createAcc(u);
        qDebug() << "Attempting Create Account...";
        state = ClientState::CreatingAccount;
    }
    else
    {
        qDebug() << "Can't Create Account. Current State:" << state;
    }
}
    
void Client::submitAuthCode(const QString& code)
{
    if (state == ClientState::CreatingAccount)
    {
        RegMachine::getInstance()->authAcc(current_user,code);
        qDebug() << "Attempting Authentication...";
    }
    else
    {
        qDebug() << "Can't Authenticate Account. Current State:" << state;
    }
}

void Client::openListener()
{
    if (state == ClientState::LoggedIn && listener == NULL)
    {
        listener = new QTcpServer(this);
        connect(listener, &QTcpServer::newConnection, this,
                &Client::onDM);    

        QHostAddress ip = getDeviceIpAddress();
        
        if (listener->listen(ip, 0))
        {
            qDebug() << "Announcing Ip and Port:"
                     << listener->serverAddress().toString()
                     << ","
                     << listener->serverPort();
            
            socket->write(
                ProtocolManager::serialize(
                    ProtocolManager::AnnounceIpPort, {
                        current_user.get_username(),
                        listener->serverAddress().toString(),
                        QString::number(listener->serverPort()) }
                    )
                );
        }
        else qDebug() << "ERROR: Listener could not start!";
    }
    else if (listener != NULL)
    {
        qDebug() << "Already announced ip and port.";
    }
    else
    {
        qDebug() << "Can't Announce Ip and port. Current state:"
                 << state; 
    }
}

void Client::closeListener()
{
    if (state == ClientState::LoggedIn && listener != NULL)
    {
        qDebug() << "Closing Listener...";
        QObject::disconnect(listener, &QTcpServer::newConnection, this,
                            &Client::onDM);    
        delete listener;
        listener = NULL;

        socket->write(
            ProtocolManager::serialize(
                ProtocolManager::AnnounceOffline, {
                    current_user.get_username() }
                )
            );
    }
    else if (listener == NULL)
    {
        qDebug() << "Listener already closed.";
    }
    else
    {
        qDebug() << "Can't close listener. Current State:"
                 << state;
    }
}

void Client::discover(const User& u)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Attempting discovery for" << u.get_username();
        
        socket->write(ProtocolManager::serialize(
                          ProtocolManager::DiscoveryRequest, {
                              current_user.get_username(), u.get_username()
                          })
            );
    }
    else
    {
        qDebug() << "Can't discover message. Current state:" << state;
    }
}

void Client::privateMessage(const User& u, const QString& message)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Attempting private message...";

        if (usermap.find(u.get_username()) != usermap.end())
        {
            qDebug() << "Already have target user information,"
                     << "continuing...";
            // forward the message to the server
            socket->write(
                ProtocolManager::serialize(
                    ProtocolManager::PrivateMessageForward, {
                        u.get_username(), current_user.get_username(),
                        message }
                    )
                );
                
            // send the message to the user
            auto ip_port = usermap[u.get_username()];
            sendDataToOtherClient(ip_port.first, ip_port.second,
                                  ProtocolManager::serialize(
                                      ProtocolManager::PrivateMessage, {
                                          u.get_username(),
                                          current_user.get_username(),
                                          message }
                                      )
                );
        }
        else
        {
            qDebug() << "Don't have target user information,"
                     << "Requesting it";
            // ask the server for the ip and port of the guy
            socket->write(ProtocolManager::serialize(
                              ProtocolManager::PrivateMessageRequest,
                              { u.get_username(),
                                current_user.get_username(),
                                message }
                              )
                );
        }
    }
    else
    {
        qDebug() << "Can't private message. Current state:" << state;
    }
}
    
void Client::onReply()
{
    QByteArray data = socket->readAll();
    qDebug() << "Received" << data << "from server.";
        
    QJsonObject m = ProtocolManager::deserialize(data);
        
    switch(state)
    {
        case ClientState::LoggedIn:
        {
            handleLoggedInState(m);
            break;
        }
        case ClientState::LoggingIn:
        {
            handleLoggingInState(m);
            break;
        }
        case ClientState::CreatingAccount:
        {
            handleCreatingAccountState(m);
            break;
        }
        default:
        {
            qDebug() << "State Failure:" << "\nCurrentState: " << state;
            break;
        }
    }
}

void Client::onDM()
{
    QTcpSocket * foreignSocket = listener->nextPendingConnection();

    connect(foreignSocket, &QTcpSocket::readyRead, this, [this,
                                                          foreignSocket]()
    {
        QByteArray data = foreignSocket->readAll();
        qDebug() << "Recieved" << data;
        QJsonObject json = ProtocolManager::deserialize(data);
        emit recievedDM(json["From"].toString(),json["Message"].toString());
    });
}

QHostAddress Client::getDeviceIpAddress()
{
    if (LOCAL_MODE) return QHostAddress::LocalHost;
    else
    {
        for (const QHostAddress& addr : QNetworkInterface::allAddresses())
            if (addr.protocol() != QAbstractSocket::IPv4Protocol
                && !addr.isLoopback()) return addr;

        qDebug() << "No address found in network interface?? Returning ()";
        return QHostAddress();
    }
}

void Client::sendDataToOtherClient(const QHostAddress& ip,
                                   const quint16& port,
                                   const QByteArray & data) const
{
    return;
    
    QTcpSocket tempSocket;
    qDebug() << "Trying to connect to" << ip << "port:" << port;
    tempSocket.connectToHost(ip,port);
    
    if (!tempSocket.waitForConnected(20000))
    {
        qDebug() << "Timeout, could not send message";
        return;
    }
    
    tempSocket.write(data);
    tempSocket.flush();
        
    tempSocket.waitForBytesWritten(2000);
    tempSocket.disconnectFromHost();
}

void Client::handleLoggedInState(const QJsonObject& m)
{
    switch(m["Type"].toInt())
    {
        case ProtocolManager::DiscoveryAccept:
        {
            QStringList messagelist = m["Messages"].toString().split(":;:");
            QList<QJsonObject> messageJsonList;
            for(QString str : messagelist)
            {
                QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
                if(!d.isNull() && d.isObject())
                    messageJsonList.append(d.object());
            }
            
s            emit discoverUserSucceed(m["Username"].toString(),
                                     messageJsonList);
            break;
        }
        case ProtocolManager::DiscoveryFail:
        {
            emit discoverUserFail(m["Username"].toString());
            break;
        }
        case ProtocolManager::PrivateMessageAccept:
        {
            QHostAddress ip(m["Ip"].toString());
            QString u(m["To"].toString());
            qDebug() << "Port try1:" << m["Port"].toString();
            
            quint16 port(m["Port"].toString().toInt());
            QString message(m["Message"].toString());
                

            qDebug() << "Port try2:" << port;
            usermap[u] = {ip,port};
          
            sendDataToOtherClient(ip,port,
                                  ProtocolManager::serialize(
                                      ProtocolManager::PrivateMessage,
                                      { u, current_user.get_username(),
                                        message } )
                );
            break;
        }
        case ProtocolManager::PrivateMessageDenied:
        {
            qDebug() << "Could not send message, reason:"
                     << m["Reason"].toString();
            break;
        }
        default:
        {
            qDebug() << "State Transition Failure:"
                     << "\nCurrent State:" << state
                     << "\n  Attempting transition to:"
                     << m["Type"].toInt();
            break;
        }
    }
}
    
void Client::handleLoggingInState(const QJsonObject& m)
{
    switch(m["Type"].toInt())
    {
        case ProtocolManager::LoginAccept:
        {
            qDebug() << "Login Success!";
            state = ClientState::LoggedIn;
            User u(m["Username"].toString(),
                   m["Password"].toString());
            current_user = u;
            openListener();
            qDebug() << "Username:" << u.get_username();
            emit loginSuccess();
            break;
        }
        case ProtocolManager::LoginDenied:
        {
            qDebug() << "Login Failed!";
            state = ClientState::Connected;
            break;
        }
        default:
        {
            qDebug() << "State Transition Failure:"
                     << "\nCurrent State:" << state
                     << "\n  Attempting transition to:"
                     << m["Type"].toInt();
            state = ClientState::Connected;
            break;
        }
    }
}
    
void Client::handleCreatingAccountState(const QJsonObject& m)
{
    switch(m["Type"].toInt())
    {
        case ProtocolManager::AccountAuthenticated:
        {
            // qDebug() << "Account"
            //          << current_user.get_username()
            //          << "Authenticated";
            state = ClientState::Connected;
            
            emit accountAuthenticated();
            break;
        }
        case ProtocolManager::AccountNotAuthenticated:
        {
            // qDebug() << "Authentication Failed, wrong code";
            emit accountAuthenticationFail();
            break;
        }
        case ProtocolManager::CreateAccountAccept:
        {
            User u(m["Username"].toString(),
                   m["Password"].toString());
            // qDebug() << "Account with username"
            //          << u.get_username()
            //          << "created!";
            current_user = u;
            emit accountCreated();
            break;
        }
        case ProtocolManager::CreateAccountDenied:
        {
            //qDebug() << "Account Creation Failed!";
            emit accountNotCreated();
            state = ClientState::Connected;
            break;
        }
        default:
        {
            qDebug() << "State Transition Failure:"
                     << "\nCurrent State:" << state
                     << "\n  Attempting transition to:"
                     << m["Type"].toInt();
            break;
        }
    }
}
