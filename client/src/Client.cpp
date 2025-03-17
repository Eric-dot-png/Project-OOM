// file : Client.cpp
// name : eric garcia
// Description: This file implements the Client class in a Discord-like chat application using Qt as the graphical interface.
//              It handles the communication with the server (connection, disconnection, sending requests, and receiving responses),
//              manages user authentication (login, account creation, etc.), and maintains different client states to reflect the
//              current stage of interaction with the server.

#include "Client.h"
#include "regMachine.h"
#include "User.h"

// Static member holding the singleton instance of Client
Client * Client::instance(NULL);
 
// Constructor: Initializes the Client object with default states, socket, and sets a guest user.
Client::Client()
    : QObject(NULL), state(DisconnectedState::getInstance()),
      socket(new QTcpSocket(this)),
      current_user("None","None","None") // for now this is guest user
{
    // When the socket is successfully connected to the server, update the state to ConnectedState.
    connect(socket, &QTcpSocket::connected, this, [&](){
        qDebug() << "Connected to Server";
        state = ConnectedState::getInstance();
        emit connectedToServer();
    });

    // When the socket is disconnected from the server, update the state to DisconnectedState.
    connect(socket, &QTcpSocket::disconnected, this, [&](){
        qDebug() << "Disconnected from Server";
        state = DisconnectedState::getInstance();
        emit disconnectedFromServer();
    });

    // When the socket has data ready to be read, call onReply() to process the server's message.
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReply);
}

// Destructor: If not disconnected, disconnects from the server and cleans up resources.
Client::~Client()
{
    if (state != DisconnectedState::getInstance())
        disconnect();
    destroyClientStates();
    delete socket;
}

// Retrieves the singleton instance of this Client class, creating it if needed.
Client * Client::getInstance()
{
    if (instance == NULL)
        instance = new Client();
    return instance;
}

// Destroys the singleton instance, deleting it and resetting to NULL.
void Client::destroyInstance()
{
    if (instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

// Attempts to connect to the server using the specified host address and port, transitioning to ConnectingState.
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

// Disconnects from the server if currently connected, transitioning to DisconnectingState.
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

// Serializes and writes data to the server using the specified protocol and arguments.
void Client::writeToServer(Protocol type, const QList<QJsonValue>& argv)
{
    socket->write(ProtocolManager::serialize(type,argv));
}

// Sends a login request to the server with the given user's credentials, switching to LoggingInState.
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

// Sends an account creation request for the given user, using the regMachine for the creation process.
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

// Submits an authentication code (e.g., for email verification) in CreatingAccountState.
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

// Requests to discover or retrieve additional information about another user.
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

// Sends a private message to another user on the server.
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

// Sends a friend request from the current user to another user.
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

// Accepts a friend request from another user.
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

// Placeholder to extend a user's message history in the UI or logs.
void Client::extendMessageHistory(const User& u, unsigned int currentSize)
{
    qDebug() << "not implimented yet";
}

// Called when there is data available from the server, deserializes the data, and passes it to the current state.
void Client::onReply()
{
    QByteArray data = socket->readAll();
    qDebug() << "Received" << data << "from server.";

    QJsonObject m = ProtocolManager::deserialize(data);

    state->handle(m);
}

// Requests the list of pending friend requests for the given user.
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

// Requests the list of all friends for the given user.
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

// Base state function: Called when a state cannot handle the given message.
void Client::AbstractState::StateFailure(const QJsonObject& m)
{
    qDebug() << "Cannot Process " << m["Type"].toInt();
}

// DisconnectedState, DisconnectingState, ConnectingState, ConnectedState:
// These states do not process any messages, so they default to calling StateFailure.

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

// LoggingInState: Processes server responses to a login attempt.
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

// LoggedInState: Processes messages related to friend requests, direct messages, discovery, etc.
void Client::LoggedInState::handle(const QJsonObject & m)
{
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    switch(type)
    {
        case Protocol::FriendListAccept:
        {
            // Retrieve the list of friends from the server.
            QString usr = m["From"].toString();
            QStringList list = {};
            if (m["List"].isArray()) {
                QJsonArray jarray = m["List"].toArray();
                for (const QJsonValue &value : jarray)
                {
                    if (value.isString())
                    {
                        list.append(value.toString());
                    }
                }
            }
            else
            {
                qDebug() << "List not retrieved...";
                break;
            }
            emit Client::getInstance()->sendFriendList(usr, list);
            break;
        }
        case Protocol::FriendRequestListAccept:
        {
            // Retrieve the list of pending friend requests.
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
            // Could handle error messages if needed.
            break;
        case Protocol::FriendRequest:
        {
            // The current user has received a friend request.
            emit Client::getInstance()->recievedFriendRequest(m["From"].toString());
            break;
        }
        case Protocol::FriendRemoved:
        {
            // The current user has been removed from someone's friend list.
            emit Client::getInstance()->recievedFriendRemove(m["From"].toString());
            break;
        }
        case Protocol::PrivateMessage:
        {
            // A direct message from another user.
            emit Client::getInstance()->recievedDM(m["From"].toString(),
                                                   m["Message"].toString());
            break;
        }
        case Protocol::DiscoveryAccept:
        {
            // Discovery request succeeded, handle the returned message data.
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
            // Discovery request failed.
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

// CreatingAccountState: Processes server responses to a new account creation and authentication code.
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

// Cleans up and destroys all existing states.
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