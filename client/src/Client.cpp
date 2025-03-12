// file : Client.cpp
// name : eric garcia

#include "Client.h"
#include "regMachine.h"
#include "User.h"

Client * Client::instance(NULL);
 
Client::Client()
    : QObject(NULL), state(DisconnectedState::getInstance()),
      socket(new QTcpSocket(this)),
      current_user("None","None","None") // for now this is guest user
{
    connect(socket, &QTcpSocket::connected, this, [&](){
        qDebug() << "Connected to Server";
        state = ConnectedState::getInstance();
        emit connectedToServer();
    });
    
    connect(socket, &QTcpSocket::disconnected, this, [&](){
        qDebug() << "Disconnected from Server";
        state = DisconnectedState::getInstance();
        emit disconnectedFromServer();
    });
        
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReply);
}
    
Client::~Client()
{
    if (state != DisconnectedState::getInstance())
        disconnect();
    destroyClientStates();
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
    if (state == DisconnectedState::getInstance())
    {
        qDebug() << "Attempting to connect to server...";
        socket->connectToHost(host,port);
        state = ConnectingState::getInstance();
    }
    else
    {
        qDebug() << "Already connected to server.";
    }
}

void Client::disconnect()
{
    if (state != DisconnectedState::getInstance())
    {
        qDebug() << "Disconnecting from server...";
        socket->disconnectFromHost();
        state = DisconnectingState::getInstance();
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
    if (state == ConnectedState::getInstance())
    {
        qDebug() << "Attempting Login...";
        writeToServer(Protocol::LoginRequest,
                      {u.get_username(), u.get_password()});
        state = LoggingInState::getInstance();
    }
    else
    {
        qDebug() << "Can't Login. Current State:" << state;
    }
}
    
void Client::createAccount(const User & u)
{
    if (state == ConnectedState::getInstance())
    {
        RegMachine::getInstance()->createAcc(u);
        qDebug() << "Attempting Create Account...";
        state = CreatingAccountState::getInstance();
    }
    else
    {
        qDebug() << "Can't Create Account. Current State:" << state;
    }
}
    
void Client::submitAuthCode(const QString& code)
{
    if (state == CreatingAccountState::getInstance())
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
    if (state == LoggedInState::getInstance())
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
    if (state == LoggedInState::getInstance())
    {
        qDebug() << "sending private message...";
        
        writeToServer(Protocol::PrivateMessage,
            {u.get_username(), current_user.get_username(), message});
    }
    else
    {
        qDebug() << "Can't private message. Current state:" << state;
    }
}

void Client::friendRequest(const User& u)
{
    if (state == LoggedInState::getInstance())
    {
        qDebug() << "Sending Friend Request";

        writeToServer(Protocol::FriendRequest,
                      {current_user.get_username(), u.get_username()});
    }
    else
    {
        qDebug() << "Can't Friend Request. current state:" << state; 
    }
}

void Client::acceptFriend(const User& u)
{
    if (state == LoggedInState::getInstance())
    {
        qDebug() << "Accepting Friend Request";
        
        writeToServer(Protocol::FriendAccept,
                      {current_user.get_username(), u.get_username()});
    }
    else
    {
        qDebug() << "Can't Accept Friend Request. current state:" << state; 
    }
}

void Client::extendMessageHistory(const User& u, unsigned int currentSize)
{
    qDebug() << "not implimented yet";
}

void Client::onReply()
{
    QByteArray data = socket->readAll();
    qDebug() << "Received" << data << "from server.";
    
    QJsonObject m = ProtocolManager::deserialize(data);

    state->handle(m);
}

void Client::getFriendRequestList(const User &u)
{
    if (state == LoggedInState::getInstance())
    {
        qDebug() << "Getting friend Request List.";

        writeToServer(Protocol::FriendRequestList,
                      {u.get_username(), ""});
    }
    else
    {
        qDebug() << "Can't get friend request list. Current state:" << state;
    }
}

void Client::getFriendsList(const User &u)
{
    if (state == LoggedInState::getInstance())
    {
        qDebug() << "Getting friend list.";

        writeToServer(Protocol::FriendList,
                      {u.get_username(), ""});
    }
    else
    {
        qDebug() << "Can't get friend list. Current state:" << state;
    }
}

/*------------------------------------------------
Client State Stuff
------------------------------------------------*/

void Client::AbstractState::StateFailure(const QJsonObject& m)
{
    qDebug() << "Cannot Process " << m["Type"].toInt();
}

void Client::DisconnectedState::handle(const QJsonObject & m)
{
    StateFailure(m);
}

void Client::DisconnectingState::handle(const QJsonObject & m)
{
    StateFailure(m);
}

void Client::ConnectingState::handle(const QJsonObject & m)
{
    StateFailure(m);
}

void Client::ConnectedState::handle(const QJsonObject & m)
{
    StateFailure(m);
}

void Client::LoggingInState::handle(const QJsonObject & m)
{
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::LoginAccept:
        {
            qDebug() << "Login Success!";
            Client::getInstance()->state = LoggedInState::getInstance();
            User u(m["Username"].toString(),
                   m["Password"].toString());
            Client::getInstance()->current_user = u;
            qDebug() << "Username:" << u.get_username();
            emit Client::getInstance()->loginSuccess();
            break;
        }
        case Protocol::LoginDenied:
        {
            qDebug() << "Login Failed!";
            Client::getInstance()->state = ConnectedState::getInstance();
            emit Client::getInstance()->loginFail();
            break;
        }
        default:
        {
            StateFailure(m);
            Client::getInstance()->state = ConnectedState::getInstance();
            break;
        }
    }
}

void Client::LoggedInState::handle(const QJsonObject & m)
{
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::FriendListAccept:
        {
            //qDebug() << "Protocol::FriendListAccept";
            QString usr = m["From"].toString();
            QStringList list = {};
            if (m["List"].isArray()) {
                QJsonArray jarray = m["List"].toArray();
                for (const QJsonValue &value : jarray)
                {

                    if (value.isString())
                    {
                        //qDebug() << value.toString();
                        list.append(value.toString());
                    }
                }
            }
            else
            {
                qDebug() << "List not retrieved...";
                break;
            }
            //qDebug() << "List:" << list;
            emit Client::getInstance()->sendFriendList(usr, list);
            break;
        }
        case Protocol::FriendRequestListAccept:
        {
            //qDebug() << "Protocol::FriendRequestListAccept";
            QString usr = m["From"].toString();
            QStringList list;
            if (m["List"].isArray()) {
                QJsonArray jarray = m["List"].toArray();
                for (const QJsonValue &value : jarray)
                {
                    if (value.isString())
                        list.append(value.toString());
                }
            }
            else
            {
                qDebug() << "List not retrieved...";
                break;
            }
            emit Client::getInstance()->sendFriendRequestList(usr, list);
            break;
        }
        case Protocol::FriendRequestListFailed:
        case Protocol::FriendListFailed:
            break;
        case Protocol::FriendRequest:
        {
            emit Client::getInstance()->recievedFriendRequest(m["From"].toString());
            break;
        }
        case Protocol::FriendRemoved:
        {
            emit Client::getInstance()->recievedFriendRemove(m["From"].toString());
            break;
        }
        case Protocol::PrivateMessage:
        {
            emit Client::getInstance()->recievedDM(m["From"].toString(),
                                                   m["Message"].toString());
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
            
            emit Client::getInstance()->discoverUserSucceed(m["Username"].toString(), messageJsonList);
            break;
        }
        case Protocol::DiscoveryFail:
        {
            emit Client::getInstance()->discoverUserFail(m["Username"].toString());
            break;
        }
        default:
        {
            StateFailure(m);
            break;
        }
    }
}

void Client::CreatingAccountState::handle(const QJsonObject & m)
{
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::AuthCodeAccept:
        {
            Client::getInstance()->state = Client::ConnectedState::getInstance();
            emit Client::getInstance()->accountAuthenticated();
            break;
        }
        case Protocol::AuthCodeDenied:
        {
            emit Client::getInstance()->accountAuthenticationFail();
            break;
        }
        case Protocol::CreateAccountAccept:
        {
            emit Client::getInstance()->accountCreated();
            Client::getInstance()->current_user = User(m["Username"].toString(),
                                                     m["Password"].toString());
            break;
        }
        case Protocol::CreateAccountDenied:
        {
            emit Client::getInstance()->accountNotCreated();
            Client::getInstance()->state = Client::ConnectedState::getInstance();
            break;
        }
        default:
        {
            StateFailure(m);
            break;
        }
    }
}

void Client::destroyClientStates()
{
    DisconnectedState::destroyInstance();
    DisconnectingState::destroyInstance();
    ConnectingState::destroyInstance();
    ConnectedState::destroyInstance();
    LoggingInState::destroyInstance();
    LoggedInState::destroyInstance();
    CreatingAccountState::destroyInstance();
}
