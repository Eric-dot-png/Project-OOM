#include "Client.h"

Client::Client()
    : nw(NetworkManager::getInstance()),
      state(DisconnectedState::getInstance()),
      current_user(GUEST)
{
    connect(nw, &NetworkManager::connected, this, [&](){
        state = ConnectedState::getInstance();
        emit connectedToServer();
    });

    connect(nw, &NetworkManager::disconnected, this, [&](){
        state = DisconnectedState::getInstance();
        emit disconnectedFromServer();
    });
    
    connect(nw, &NetworkManager::loginValid, this, &Client::initializeSession);
    
    connect(nw, &NetworkManager::loginInvalid, this, [&](){
        state = ConnectedState::getInstance();
        qDebug() << "Client : Entered Connected State";
        emit loginFail();
    });

    connect(nw, &NetworkManager::accSetupFail, this, [&](){
        state = ConnectedState::getInstance();
        emit accountNotCreated();
    });
    
    connect(nw, &NetworkManager::accSetupPass, this, [&](const QString& usr, const QString& pwd){
        current_user = User(usr,pwd);
        emit accountCreated();
    });

    connect(nw, &NetworkManager::accAuthPass, this, [&](){
        state = ConnectedState::getInstance();
        emit accountAuthenticated();
    });
    
    connect(nw, &NetworkManager::accAuthFail, this, [&](){
        emit accountAuthenticationFail();
    });
    
    connect(nw, &NetworkManager::userDNE, this, [&](const QString& u){
        emit discoverUserFail(u);
    });

    connect(nw, &NetworkManager::pmHistoryFound, this,&Client::initializeDMs);
    
    connect(nw, &NetworkManager::detectedPM, this, [&](const QString& u, const QString& msg){
        emit recievedDM(u,msg);
    });

    connect(nw, &NetworkManager::detectedFriendReq, this, [&](const QString& u){
        emit recievedFriendRequest(u);
    });

    connect(nw, &NetworkManager::detectedFriendRM, this, [&](const QString& u){
        emit recievedFriendRemove(u);
    });
}

Client::~Client()
{
    nw->disconnect();
    destroyClientStates();
}

// Attempts to connect to the server using the specified host address and port, transitioning to ConnectingState.
void Client::connectToServer(const QHostAddress& host, int port)
{
    if (state == DisconnectedState::getInstance())
    {
        qDebug() << "Attempting to connect to server...";
        nw->connect();
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
        nw->disconnect();
        state = DisconnectingState::getInstance();
    }
    else
    {
        qDebug() << "Already disconnected.";
    }
}

// Sends a login request to the server with the given user's credentials, switching to LoggingInState.
void Client::login(const User & u)
{
    if (state == ConnectedState::getInstance())
    {
        qDebug() << "Attempting Login...";
        nw->validateLogin(u.get_username(), u.get_password());
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
        nw->setupAcc(u.get_username(), u.get_password(), u.get_email());
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
        nw->authAcc(current_user.get_username(),
                    current_user.get_password(), code);
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
        nw->pmHistory(current_user.get_username(), u.get_username());
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
        nw->forwardPM(u.get_username(), current_user.get_username(), message);
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
        nw->forwardFriendReq(u.get_username(), current_user.get_username());
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

        qDebug() << "Mega Broken!";
    }
    else
    {
        qDebug() << "Can't Accept Friend Request. current state:" << state; 
    }
}

// Placeholder to extend a user's message history in the UI or logs.
void Client::extendMessageHistory(const User& u, quint32 currentSize)
{
    //qDebug() << "not implimented yet";
    if (state == LoggedInState::getInstance())
    {
        qint64 size = static_cast<qint64>(currentSize);
        qDebug() << "Extending message history";
        // writeToServer(Protocol::ExtendMessageHistory,
        //               {current_user.get_username(), u.get_username(), size});
    }
    else
    {
        qDebug() << "Can't Extend message history, not in logged in state";
    }
}

// Requests the list of pending friend requests for the given user.
void Client::getFriendRequestList(const User &u)
{
    if (state == LoggedInState::getInstance())
    {
        qDebug() << "Getting friend Request List.";
        // writeToServer(Protocol::FriendRequestList,
        //               {u.get_username(), ""});
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
    }
    else
    {
        qDebug() << "Can't get friend list. Current state:" << state;
    }
}

void Client::initializeSession(const QString& user,
                               const QStringList& fs,
                               const QStringList& frs)
{
    state = LoggedInState::getInstance();
    current_user = User(user);
    current_user.setFriendList(fs);
    current_user.setFriendRequestList(frs);
    emit loginSuccess();
}

void Client::initializeDMs(const QString& user, const QString& msgs)
{
    QStringList messagelist = msgs.split(":;:");
    QList<QJsonObject> messageJsonList;
    for(const QString& str : messagelist)
    {
        QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
        if(!d.isNull() && d.isObject())
            messageJsonList.append(d.object());
    } 
    emit discoverUserSucceed(user, messageJsonList);
}

// Base state function: Called when a state cannot handle the given message.
void Client::AbstractState::StateFailure(const QJsonObject& m)
{
    qDebug() << "Cannot Process " << m["Type"].toInt();
}

// DisconnectedState, DisconnectingState, ConnectingState, ConnectedState:
// These states do not process any messages, so they default to calling
// StateFailure.
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
        case Protocol::ExtendMessageHistoryAccept:
        {
            qDebug() << "Recieved extension.";
            QStringList messagelist = m["Messages"].toString().split(":;:");
            QList<QJsonObject> msgs;
            for(QString str : messagelist)
            {
                QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
                if(!d.isNull() && d.isObject())
                    msgs.append(d.object());
            }
            emit Client::getInstance()->extendMsgSucceed(m["Username"].toString(), msgs);
            break;
        }
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
        case Protocol::ExtendMessageHistoryDenied:
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
        case Protocol::FriendAccept:
        {
            
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
