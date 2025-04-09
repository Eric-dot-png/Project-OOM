#include "Client.h"

Client::Client()
    : nw(NetworkManager::getInstance()),
      state(ClientState::Disconnected),
      current_user(GUEST)
{
    connect(nw, &NetworkManager::connected, this, [&](){
        state = ClientState::Connected;
        emit connectedToServer();
    });

    connect(nw, &NetworkManager::disconnected, this, [&](){
        state = ClientState::Disconnected;
        emit disconnectedFromServer();
    });
    
    connect(nw,&NetworkManager::loginValid, this, &Client::initializeSession);
    
    connect(nw, &NetworkManager::loginInvalid, this, [&](){
        state = ClientState::Connected;
        qDebug() << "Client : Entered Connected State";
        emit loginFail();
    });

    connect(nw, &NetworkManager::accSetupFail, this, [&](){
        state = ClientState::Connected;
        emit accountNotCreated();
    });
    
    connect(nw, &NetworkManager::accSetupPass, this, [&](const QString& usr,
                                                         const QString& pwd){
        current_user = User(usr,pwd);
        emit accountCreated();
    });

    connect(nw, &NetworkManager::accAuthPass, this, [&](){
        state = ClientState::Connected;
        emit accountAuthenticated();
    });
    
    connect(nw, &NetworkManager::accAuthFail, this, [&](){
        emit accountAuthenticationFail();
    });
    
    connect(nw, &NetworkManager::userDNE, this, [&](const QString& u){
        emit discoverUserFail(u);
    });
    
    connect(nw, &NetworkManager::pmHistoryFound,this,&Client::initializeDMs);
    
    connect(nw, &NetworkManager::detectedPM, this, &Client::handleDM);
    
    connect(nw, &NetworkManager::detectedFriendReq, this, [&](const QString& u){
        friendlist.recieveRequest(u);
        current_user.addFriendRequest(u);
        emit recievedFriendRequest(u);
    });

    connect(nw, &NetworkManager::detectedFriendAccept, this, [&](const QString& u){
        current_user.removeFriendRequest(u);
        current_user.addFriend(u);
        friendlist.requestAccepted(u);
        emit friendAccepted(u);
    });

    connect(nw, &NetworkManager::detectedFriendDeny, this, [&](const QString& u) {
        // do nothing (remove from outgoing friendrequest?)
        friendlist.requestDenied(u);
    });
    
    connect(nw, &NetworkManager::detectedFriendRM, this, [&](const QString& u){
        current_user.removeFriend(u);
        friendlist.unfriend(u);
        emit recievedFriendRemove(u);
    });

    connect(nw, &NetworkManager::moreMessages, this, &Client::handleMoreMsgs);

    connect(nw, &NetworkManager::createGroupPass, this, [&](const QString & name, const QStringList & members) {
        std::unordered_set<QString> m;
        for(const QString & s : members)
            m.insert(s);
        initializeGroups(current_user.get_username(), name, m, {});
        emit createGroupSucceed(name, members);
    });

    connect(nw, &NetworkManager::createGroupFail, this, [&](const QString & err) {
        emit createGroupDeny(err);
    });

    connect(nw, &NetworkManager::detectedGroupMessage, this, &Client::handleGroupMessage);
        
    connect(qApp, &QCoreApplication::aboutToQuit, this, &Client::logout);
}

Client::~Client()
{
    nw->disconnect();
}

// Attempts to connect to the server using the specified host address and port, transitioning to ConnectingState.
void Client::connectToServer(const QHostAddress& host, int port)
{
    if (state == ClientState::Disconnected)
    {
        qDebug() << "Attempting to connect to server...";
        nw->connect();
        state = ClientState::Connecting;
    }
    else
    {
        qDebug() << "Already connected to server.";
    }
}

// Disconnects from the server if currently connected, transitioning to DisconnectingState.
void Client::disconnect()
{
    if (state != ClientState::Disconnected)
    {
        qDebug() << "Disconnecting from server...";
        nw->disconnect();
        state = ClientState::Disconnecting;
    }
    else
    {
        qDebug() << "Already disconnected.";
    }
}

// Sends a login request to the server with the given user's credentials, switching to LoggingInState.
void Client::login(const User & u)
{
    if (state == ClientState::Connected)
    {
        qDebug() << "Attempting Login...";
        nw->validateLogin(u.get_username(), u.get_password());
        state = ClientState::LoggingIn;
    }
    else
    {
        qDebug() << "Can't Login. Current State:" << static_cast<int>(state);
    }
}

void Client::logout()
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Attempting Logout";
        nw->logoutFrom(current_user.get_username());
        nw->flush();
        current_user = GUEST;
    }
    else
    {
        qDebug() << "Can't Logout. Current State:" << static_cast<int>(state);
    }
}

// Sends an account creation request for the given user, using the regMachine for the creation process.
void Client::createAccount(const User & u)
{
    if (state == ClientState::Connected)
    {
        nw->setupAcc(u.get_username(), u.get_password(), u.get_email());
        qDebug() << "Attempting Create Account...";
        state = ClientState::CreatingAccount;
    }
    else
    {
        qDebug() << "Can't Create Account. Current State:" << static_cast<int>(state);
    }
}

// Submits an authentication code (e.g., for email verification) in CreatingAccountState.
void Client::submitAuthCode(const QString& code)
{
    if (state == ClientState::CreatingAccount)
    {
        nw->authAcc(current_user.get_username(),
                    current_user.get_password(), code);
        qDebug() << "Attempting Authentication...";
    }
    else
    {
        qDebug() << "Can't Authenticate Account. Current State:" << static_cast<int>(state);
    }
}

// Requests to discover or retrieve additional information about another user.
void Client::discover(const User& u)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Attempting discovery for" << u.get_username();
        nw->pmHistory(current_user.get_username(), u.get_username());
    }
    else
    {
        qDebug() << "Can't discover message. Current state:" << static_cast<int>(state);
    }
}

// Sends a private message to another user on the server.
void Client::privateMessage(const User& u, const QString& message)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "sending private message...";
        nw->forwardPM(u.get_username(), current_user.get_username(), message);
        
        if (chats.find(dmKey(u)) != chats.end())
        {
            Message m(current_user.get_username(), u.get_username(), message);
            chats[dmKey(u)]->prepend(m);
        }
    }
    else
    {
        qDebug() << "Can't private message. Current state:" << static_cast<int>(state);
    }
}

// Sends a friend request from the current user to another user.
void Client::friendRequest(const User& u)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Sending Friend Request";
        friendlist.sendRequest(u.get_username());
        nw->forwardFriendReq(u.get_username(), current_user.get_username());
    }
    else
    {
        qDebug() << "Can't Friend Request. current state:" << static_cast<int>(state); 
    }
}

// Accepts a friend request from another user.
void Client::acceptFriend(const User& u)
{
    if (state == ClientState::LoggedIn &&
        current_user.getFriendRequestList().contains(u.get_username()))
    {
        qDebug() << "Accepting Friend Request";
        current_user.removeFriendRequest(u.get_username());
        current_user.addFriend(u.get_username());
        friendlist.acceptRequest(u.get_username());
        nw->forwardFriendAccept(u.get_username(),current_user.get_username());
    }
    else if (state != ClientState::LoggedIn)
    {
        qDebug() << "Can't Accept Friend Request. current state:" << static_cast<int>(state); 
    }
    else
    {
        qDebug() << u.get_username() << " is not requesting to be friend.";
    }
}

void Client::denyFriend(const User& u)
{
    if (state == ClientState::LoggedIn &&
        current_user.getFriendRequestList().contains(u.get_username()))
    {
        qDebug() << "Denying Friend Request";
        current_user.removeFriendRequest(u.get_username());
        friendlist.denyRequest(u.get_username());
        nw->forwardFriendDeny(u.get_username(), current_user.get_username());
    }
    else if (state != ClientState::LoggedIn)
    {
        qDebug() << "Can't Deny Friend Request. current state:" << static_cast<int>(state); 
    }
    else
    {
        qDebug() << u.get_username() << " is not requesting to be friend.";
    }
}

// Removes a friend from both users (this one and u)
void Client::removeFriend(const User& u)
{
    if (state == ClientState::LoggedIn &&
        current_user.getFriendsList().contains(u.get_username()))
    {
        qDebug() << "Removing Friend";
        current_user.removeFriend(u.get_username());
        nw->forwardFriendRemove(u.get_username(),current_user.get_username());
    }
    else if (state != ClientState::LoggedIn)
    {
        qDebug() << "Can't remove friend. current state:" << static_cast<int>(state);
    }
    else
    {
        qDebug() << u.get_username() << "is not on friendslist.";
    }
}

void Client::block(const User& u)
{
    if (state == ClientState::LoggedIn)
    {
        blocklist.add(u.get_username());
        nw->forwardBlock(u.get_username(), current_user.get_username());
    }
    else
    {
        qDebug() << "Can't block, not in loggedin state.";
    }
}

void Client::unblock(const User& u)
{
    if (state == ClientState::LoggedIn)
    {
        blocklist.remove(u.get_username());
        nw->forwardUnblock(u.get_username(), current_user.get_username());
    }
    else
    {
        qDebug() << "Can't block, not in loggedin state.";
    }
}

// Placeholder to extend a user's message history in the UI or logs.
void Client::extendMessageHistory(const User& u, quint32 currentSize)
{
    if (state == ClientState::LoggedIn)
    {
        qDebug() << "Extending message history";
        nw->requestMoreMessages(u.get_username(), current_user.get_username(),
                                currentSize);
    }
    else
    {
        qDebug() << "Can't Extend message history, not in logged in state";
    }
}

void Client::extendMessageHistory(const User& u)
{
    extendMessageHistory(u, chats[dmKey(u)]->size());
}


const ChatObject * Client::getDMsWith(const QString& u) const
{
    qDebug() << "getting dms with" << dmKey(u);
    auto pair = chats.find(dmKey(u));
    if (pair != chats.end()) return pair->second;
    else
    {
        throw std::runtime_error("Client getDM ERROR: User not discovered."); 
    }
}


void Client::initializeSession(const QString& user,
                               const QStringList& fs,
                               const QStringList& frs,
                               const QJsonArray & groups)
{
    qDebug() << "Intiailizing Session(user, friends, friend reqs, groups)...";
    state = ClientState::LoggedIn;
    current_user = User(user);
    current_user.setFriendList(fs);
    current_user.setFriendRequestList(frs);
    for(const QJsonValue & v : groups)
    {
        QJsonArray memarray = v["Members"].toArray();
        std::unordered_set<QString> members;
        for(const auto mem : memarray)
            members.insert(mem.toString());
        initializeGroups(v["Owner"].toString(), v["GroupName"].toString(),
                         members, {});
    }
    emit loginSuccess();
    qDebug() << "Session initialized.";
}

void Client::initializeDMs(const QString& user, const QJsonArray & messages)
{
    qDebug() << "Initializing dms for" << dmKey(user);
    if (chats.find(dmKey(user)) == chats.end())
    {
        // this chat hasn't been initialized, so initialize it
        chats[dmKey(user)] = new DirectMessage(
            current_user.get_username(), user, this);
        for (const QJsonValue& msg : messages)
        {
            Message m(msg["From"].toString(), msg["To"].toString(),
                      msg["Message"].toString());
            chats[dmKey(user)]->sendMessage(m);
        }
    } // otherwise dont overwrite chat
    emit discoverUserSucceed(user, messages);
    qDebug() << "Dms initialized";
}

void Client::initializeGroups(const QString & owner, const QString & name,
                              const std::unordered_set<QString> & members,
                              const QJsonArray & messages)
{
    qDebug() << "Initializing group...";
    if(chats.find(groupKey(owner, name)) != chats.end())
    {
        chats[groupKey(owner, name)] = new Group(members, owner, name, this);
        for(const QJsonValue & msg : messages)
        {
            Message m(msg["From"].toString(), name, msg["Message"].toString());
            chats[groupKey(owner, name)]->sendMessage(m);
        }
    }
}

void Client::handleDM(const QString& user, const QString& msg)
{
    if (chats.find(dmKey(user)) != chats.end())
    {
        Message m(user, current_user.get_username(), msg);
        chats[dmKey(user)]->prepend(m);
    }
    emit recievedDM(user,msg);
}

void Client::handleGroupMessage(const QString & owner, const QString & name,
                                const QString & from, const QString & message)
{
    if(chats.find(groupKey(owner, name)) != chats.end())
    {
        Message m(from, name, message);
        chats[groupKey(owner, name)]->prepend(m);
    }
    emit recievedGroupMessage(owner, name, from, message);
}

void Client::handleMoreMsgs(const QString& user, const QJsonArray & messages)
{
    for (const QJsonValue& msg : messages)
    {
        Message m(msg["From"].toString(),
                  msg["To"].toString(),
                  msg["Mesage"].toString());
        chats[dmKey(user)]->sendMessage(m);
    }
}

void Client::createGroup(const QString & name, const QStringList & members) const
{
    nw->forwardCreateGroup(current_user.get_username(), name, members);
}

void Client::messageGroup(const QString & owner, const QString & name,
                          const QString & message) const
{
    nw->forwardGroupMessage(owner, name, current_user.get_username(),
                            message);
}

QString Client::dmKey(const QString& user) const
{
    return QString("D:") + user;
}

QString Client::dmKey(const User& u) const
{
    return dmKey(u.get_username());
}

QString Client::groupKey(const QString & owner, const QString & name) const
{
    return QString("G:") + owner + ":" + name;
}

// // DisconnectedState, DisconnectingState, ConnectingState, ConnectedState:
// // These states do not process any messages, so they default to calling
// // StateFailure.
// void Client::DisconnectedState::handle(const QJsonObject & m)
// {
//     StateFailure(m);
// }

// void Client::DisconnectingState::handle(const QJsonObject & m)
// {
//     StateFailure(m);
// }

// void Client::ConnectingState::handle(const QJsonObject & m)
// {
//     StateFailure(m);
// }

// void Client::ConnectedState::handle(const QJsonObject & m)
// {
//     StateFailure(m);
// }

// // LoggingInState: Processes server responses to a login attempt.
// void Client::LoggingInState::handle(const QJsonObject & m)
// {
//     Protocol type = static_cast<Protocol>(m["Type"].toInt());
//     switch(type)
//     {
//         case Protocol::LoginAccept:
//         {
//             qDebug() << "Login Success!";
//             Client::getInstance()->state = LoggedInState::getInstance();
//             User u(m["Username"].toString(),
//                    m["Password"].toString());
//             Client::getInstance()->current_user = u;
//             qDebug() << "Username:" << u.get_username();
//             emit Client::getInstance()->loginSuccess();
//             break;
//         }
//         case Protocol::LoginDenied:
//         {
//             qDebug() << "Login Failed!";
//             Client::getInstance()->state = ConnectedState::getInstance();
//             emit Client::getInstance()->loginFail();
//             break;
//         }
//         default:
//         {
//             StateFailure(m);
//             Client::getInstance()->state = ConnectedState::getInstance();
//             break;
//         }
//     }
// }

// // LoggedInState: Processes messages related to friend requests, direct messages, discovery, etc.
// void Client::LoggedInState::handle(const QJsonObject & m)
// {
//     Protocol type = static_cast<Protocol>(m["Type"].toInt());
//     switch(type)
//     {
//         case Protocol::ExtendMessageHistoryAccept:
//         {
//             qDebug() << "Recieved extension.";
//             if(m["Messages"].isArray())
//             {
//                 QJsonArray messagelist = m["Messages"].toArray();
//             // for(QString str : messagelist)
//             // {
//             //     QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
//             //     if(!d.isNull() && d.isObject())
//             //         msgs.append(d.object());
//             // }
//                 emit Client::getInstance()->extendMsgSucceed(m["Username"].toString(), messagelist);
//             }
//             else
//             {
//                 qDebug() << "Messages not retrieved...";
//             }
//             break;
//         }
//         case Protocol::FriendListAccept:
//         {
//             // Retrieve the list of friends from the server.
//             QString usr = m["From"].toString();
//             QStringList list = {};
//             if (m["List"].isArray()) {
//                 QJsonArray jarray = m["List"].toArray();
//                 for (const QJsonValue &value : jarray)
//                 {
//                     if (value.isString())
//                     {
//                         list.append(value.toString());
//                     }
//                 }
//             }
//             else
//             {
//                 qDebug() << "List not retrieved...";
//                 break;
//             }
//             emit Client::getInstance()->sendFriendList(usr, list);
//             break;
//         }
//         case Protocol::FriendRequestListAccept:
//         {
//             // Retrieve the list of pending friend requests.
//             QString usr = m["From"].toString();
//             QStringList list;
//             if (m["List"].isArray()) {
//                 QJsonArray jarray = m["List"].toArray();
//                 for (const QJsonValue &value : jarray)
//                 {
//                     if (value.isString())
//                         list.append(value.toString());
//                 }
//             }
//             else
//             {
//                 qDebug() << "List not retrieved...";
//                 break;
//             }
//             emit Client::getInstance()->sendFriendRequestList(usr, list);
//             break;
//         }
//         case Protocol::ExtendMessageHistoryDenied:
//         case Protocol::FriendRequestListFailed:
//         case Protocol::FriendListFailed:
//             // Could handle error messages if needed.
//             break;
//         case Protocol::FriendRequest:
//         {
//             // The current user has received a friend request.
//             emit Client::getInstance()->recievedFriendRequest(m["From"].toString());
//             break;
//         }
//         case Protocol::FriendRemoved:
//         {
//             // The current user has been removed from someone's friend list.
//             emit Client::getInstance()->recievedFriendRemove(m["From"].toString());
//             break;
//         }
//         case Protocol::FriendAccept:
//         {
            
//             break;
//         }
//         case Protocol::PrivateMessage:
//         {
//             // A direct message from another user.
//             emit Client::getInstance()->recievedDM(m["From"].toString(),
//                                                    m["Message"].toString());
//             break;
//         }
//         case Protocol::DiscoveryAccept:
//         {
//             // Discovery request succeeded, handle the returned message data.
//             if(m["Messages"].isArray())
//             {
//                 QJsonArray messagelist = m["Messages"].toArray();
//             // QList<QJsonObject> messageJsonList;
//             // for(QString str : messagelist)
//             // {
//             //     QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
//             //     if(!d.isNull() && d.isObject())
//             //         messageJsonList.append(d.object());
//             // }
//                 emit Client::getInstance()->discoverUserSucceed(m["Username"].toString(), messagelist);
//             }
//             else
//             {
//                 qDebug() << "discovery messages not found...";
//             }
//             break;
//         }
//         case Protocol::DiscoveryFail:
//         {
//             // Discovery request failed.
//             emit Client::getInstance()->discoverUserFail(m["Username"].toString());
//             break;
//         }
//         default:
//         {
//             StateFailure(m);
//             break;
//         }
//     }
// }

// // CreatingAccountState: Processes server responses to a new account creation and authentication code.
// void Client::CreatingAccountState::handle(const QJsonObject & m)
// {
//     Protocol type = static_cast<Protocol>(m["Type"].toInt());
//     switch(type)
//     {
//         case Protocol::AuthCodeAccept:
//         {
//             Client::getInstance()->state = Client::ConnectedState::getInstance();
//             emit Client::getInstance()->accountAuthenticated();
//             break;
//         }
//         case Protocol::AuthCodeDenied:
//         {
//             emit Client::getInstance()->accountAuthenticationFail();
//             break;
//         }
//         case Protocol::CreateAccountAccept:
//         {
//             emit Client::getInstance()->accountCreated();
//             Client::getInstance()->current_user = User(m["Username"].toString(),
//                                                      m["Password"].toString());
//             break;
//         }
//         case Protocol::CreateAccountDenied:
//         {
//             emit Client::getInstance()->accountNotCreated();
//             Client::getInstance()->state = Client::ConnectedState::getInstance();
//             break;
//         }
//         default:
//         {
//             StateFailure(m);
//             break;
//         }
//     }
// }

// // Cleans up and destroys all existing states.
// void Client::destroyClientStates()
// {
//     DisconnectedState::destroyInstance();
//     DisconnectingState::destroyInstance();
//     ConnectingState::destroyInstance();
//     ConnectedState::destroyInstance();
//     LoggingInState::destroyInstance();
//     LoggedInState::destroyInstance();
//     CreatingAccountState::destroyInstance();
// }
