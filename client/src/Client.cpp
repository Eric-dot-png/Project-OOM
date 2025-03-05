// file : Client.cpp
// name : eric garcia

#include "Client.h"
#include "regMachine.h"

Client * Client::instance(NULL);
 
Client::Client(QObject * parent)
    : QObject(parent), state(ClientState::Disconnected),
      socket(new QTcpSocket(this)),
      current_user("None","None","None") // for now this is guest user
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
        socket->disconnectFromHost();
        state = Disconnecting;
    }
    else
    {
        qDebug() << "Already disconnected.";
    }
}
    
void Client::writeToServer(Protocol type, const QList<QJsonValue>& argv)
{
    socket->write(ProtocolManager::serialize(type,argv));
}

void Client::login(const User & u)
{
    if (state == ClientState::Connected)
    {
        qDebug() << "Attempting Login...";
        writeToServer(Protocol::LoginRequest,
                      {u.get_username(), u.get_password()});
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

void Client::discover(const User& u)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Attempting discovery for" << u.get_username();
        
        writeToServer(Protocol::DiscoveryRequest,
                      {current_user.get_username(), u.get_username()});
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
        
        writeToServer(Protocol::PrivateMessage,
            {u.get_username(), current_user.get_username(), message});
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

void Client::handleLoggedInState(const QJsonObject& m)
{
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::PrivateMessage:
        {
            qDebug() << "oMg i gOt a mEsSagE";
            emit recievedDM(m["From"].toString(), m["Message"].toString());
            break;
        }
        case Protocol::DiscoveryAccept:
        {
            QStringList messagelist = m["Messages"].toString().split(":;:");
            QList<QJsonObject> messageJsonList;
            for(QString str : messagelist)
            {
                QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
                if(!d.isNull() && d.isObject())
                    messageJsonList.append(d.object());
            }
            
            emit discoverUserSucceed(m["Username"].toString(),
                                     messageJsonList);
            break;
        }
        case Protocol::DiscoveryFail:
        {
            emit discoverUserFail(m["Username"].toString());
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
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::LoginAccept:
        {
            qDebug() << "Login Success!";
            state = ClientState::LoggedIn;
            User u(m["Username"].toString(),
                   m["Password"].toString());
            current_user = u;
            qDebug() << "Username:" << u.get_username();
            emit loginSuccess();
            break;
        }
        case Protocol::LoginDenied:
        {
            qDebug() << "Login Failed!";
            state = ClientState::Connected;
            emit loginFail();
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
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::AuthCodeAccept:
        {
            state = ClientState::Connected;
            emit accountAuthenticated();
            break;
        }
        case Protocol::AuthCodeDenied:
        {
            emit accountAuthenticationFail();
            break;
        }
        case Protocol::CreateAccountAccept:
        {
            emit accountCreated();
            current_user = User(m["Username"].toString(),
                                m["Password"].toString());
            break;
        }
        case Protocol::CreateAccountDenied:
        {
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
