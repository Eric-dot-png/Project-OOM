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
        if(blocklist.isBlocked(u))
        {
            denyFriend(u);
            return;
        }
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

    connect(nw, &NetworkManager::createGroupPass, this, [&](const QString & owner, const QString & name, const QStringList & members) {
        std::unordered_set<QString> m;
        for(const QString & s : members)
            m.insert(s);
        initializeGroups(owner, name, m, {});
        emit createGroupSucceed(owner, name, members);
    });

    connect(nw, &NetworkManager::createGroupFail, this,
            [&](const QString & err) {
        emit createGroupDeny(err);
    });

    connect(nw, &NetworkManager::detectedGroupMessage, this,
            &Client::handleGroupMessage);

    connect(nw, &NetworkManager::groupHistoryFound, this,
            &Client::handleGroupHistoryFound);

    connect(nw, &NetworkManager::detectedGroupMemberLeave, this,
            &Client::handleDetectedGroupMemberLeave);

    connect(nw, &NetworkManager::detectedAddGroupMember, this,
            [&](const QString & owner, const QString & name,
                const QString & user) {
                auto pair = chats.find(groupKey(owner, name));
                if(pair != chats.end())
                    pair->second->addMember(user);
                emit GroupMemberAdded(owner, name, user);
            });


    connect(nw, &NetworkManager::detectedGroupTransfer, this,
            [&](const QString & owner, const QString & name,
                const QString & user) {
                auto pair = chats.find(groupKey(owner, name));
                if(pair != chats.end())
                {
                    ChatObject * c = pair->second;
                    c->setOwner(user);
                    chats.erase(pair);
                    chats[groupKey(user, name)] = c;
                }
                emit GroupTransferred(owner, name, user);
            });

    connect(nw, &NetworkManager::detectedGroupDelete, this,
            [&](const QString & owner, const QString & name) {
                auto pair = chats.find(groupKey(owner, name));
                if(pair != chats.end())
                {
                    delete pair->second;
                    chats.erase(pair);
                }
                emit GroupDeleted(owner, name);
            });

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
        if(blocklist.isBlocked(u.get_username()))
            return;
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
        if(!blocklist.isBlocked(u.get_username()))
            return;
        blocklist.remove(u.get_username());
        nw->forwardUnblock(u.get_username(), current_user.get_username());
    }
    else
    {
        qDebug() << "Can't block, not in loggedin state.";
    }
}

BlockList Client::getBlockList()
{
    return blocklist;
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
    auto pair = chats.find(dmKey(u));
    if (pair != chats.end()) return pair->second;
    else
    {
        return nullptr; //probably bad I do a nullptr check to see if discovered or not in privateMessages
        //throw std::runtime_error("Client getDM ERROR: User not discovered.");
    }
}


void Client::initializeSession(const QString& user,
                               const QStringList& fs,
                               const QStringList& frs,
                               const QJsonArray & groups,
                               const QStringList & blocks)
{
    qDebug() << "Intiailizing Session(user, friends, friend reqs, groups)...";
    state = ClientState::LoggedIn;
    current_user = User(user);
    current_user.setFriendList(fs);
    current_user.setFriendRequestList(frs);

    friendlist = FriendList(frs, {}, fs);
    
    blocklist = BlockList(blocks);

    for(const QJsonValue & v : groups)
    {
        QJsonArray memarray = v["Members"].toArray();
        std::unordered_set<QString> members;
        for(const auto mem : memarray)
            members.insert(mem.toString());
        initializeGroups(v["Owner"].toString(), v["Name"].toString(),
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
    }
    else
    {
        chats[dmKey(user)]->clear();
    }
    for (const QJsonValue& msg : messages)
    {
        Message m(msg["From"].toString(), msg["To"].toString(),
                  msg["Message"].toString());
        chats[dmKey(user)]->sendMessage(m);
    }
    emit discoverUserSucceed(user, messages);
    qDebug() << "Dms initialized";
}

void Client::initializeGroups(const QString & owner, const QString & name,
                              const std::unordered_set<QString> & members,
                              const QJsonArray & messages)
{
    qDebug() << "Initializing group..." << groupKey(owner, name);
    if(chats.find(groupKey(owner, name)) == chats.end())
    {
        chats[groupKey(owner, name)] = new Group(members, owner, name, this);
        for(const QJsonValue & msg : messages)
        {
            Message m(msg["From"].toString(), name, msg["Message"].toString());
            if(!blocklist.isBlocked(msg["From"].toString()))
                chats[groupKey(owner, name)]->sendMessage(m);
        }
        qDebug() << "Group initialized";
    }
}

void Client::handleDM(const QString& user, const QString& msg)
{
    if(blocklist.isBlocked(user))
        return;
    Message m(user, current_user.get_username(), msg);

    if (chats.find(dmKey(user)) != chats.end())
        chats[dmKey(user)]->prepend(m);

    emit recievedDM(user,msg);
}

void Client::handleGroupMessage(const QString & owner, const QString & name,
                                const QString & from, const QString & message)
{
    if(blocklist.isBlocked(from))
        return;
    if(chats.find(groupKey(owner, name)) != chats.end())
    {
        Message m(from, name, message);
        chats[groupKey(owner, name)]->prepend(m);
    }
    emit recievedGroupMessage(owner, name, from, message);
}

void Client::handleMoreMsgs(const QString& user, const QJsonArray & messages)
{
    QList<Message> l = {};
    for (const QJsonValue& msg : messages)
    {
        if(blocklist.isBlocked(msg["From"].toString()))
            continue;
        Message m(msg["From"].toString(),
                  msg["To"].toString(),
                  msg["Message"].toString());
        chats[dmKey(user)]->sendMessage(m);
        l.append(m);
        qDebug() << "Sending messages: " << m.get_msg();
    }
    emit recievedMoreMsgs(l);
}

void Client::handleGroupHistoryFound(const QString & owner,
                                     const QString & name,
                                     const QJsonArray & messages)
{
    qDebug() << "Group Key: " << groupKey(owner, name);
    for(const QJsonValue & msg : messages)
    {
        qDebug() << "adding group message";
        if(blocklist.isBlocked(msg["From"].toString()))
            continue;
        Message m(msg["From"].toString(), name, msg["Message"].toString());
        chats[groupKey(owner, name)]->sendMessage(m);
    }
}

void Client::handleDetectedGroupMemberLeave(const QString & owner,
                                            const QString & name,
                                            const QString & user)
{
    chats[groupKey(owner, name)]->removeMember(user);
}

void Client::createGroup(const QString & name, const QStringList & members) const
{
    nw->forwardCreateGroup(current_user.get_username(), name, members);
}

void Client::messageGroup(const QString & owner, const QString & name,
                          const QString & message)
{
    Message m(current_user.get_username(), name, message);
    qDebug() << "Group Key: " << groupKey(owner, name);
    chats[groupKey(owner, name)]->sendMessage(m);
    nw->forwardGroupMessage(owner, name, current_user.get_username(),
                            message);
}

ChatObject * Client::getGroupHistory(const QString & owner,
                                     const QString & name) const
{
    qDebug() << "searching chats for Group Key: " << groupKey(owner, name);
    auto pair = chats.find(groupKey(owner, name));
    if (pair != chats.end())
    {
        if(pair->second->size() == 0)
            nw->groupHistory(owner, name, 0);
        else
            return pair->second;
    }
    else
        qDebug() << "ERROR: WHAT ARE YOU DOING??";

    return NULL;
}

void Client::extendGroupHistory(const QString & owner,
                                const QString & name) const
{
    auto g = chats.find(groupKey(owner, name));
    if(g != chats.end())
        nw->groupHistory(owner, name, g->second->size());
}

void Client::leaveGroup(const QString & owner, const QString & name)
{
    auto g = chats.find(groupKey(owner, name));
    if(g != chats.end())
    {
        if(g->second->owner() == current_user.get_username())
        {
            qDebug() << "You made this bed, now lie in it";
            return;
        }
        chats.erase(g);
        nw->forwardLeaveGroup(owner, name, current_user.get_username());
    }
}

void Client::addGroupMember(const QString & owner, const QString & name,
                            const QString & user)
{
    if(owner != current_user.get_username())
    {
        qDebug() << "You are not the owner of this group.";
        return;
    }
    auto g = chats.find(groupKey(owner, name));
    if(g != chats.end())
    {
        if(g->second->getMembers().count(user) == 0)
        {
            g->second->addMember(user);
            nw->forwardAddGroupMember(owner, name, user);
        }
    }
}

void Client::transferGroupOwnership(const QString & owner,
                                    const QString & name,
                                    const QString & user)
{
    if(owner != current_user.get_username())
    {
        qDebug() << "You are not the owner of this group.";
        return;
    }
    auto g = chats.find(groupKey(owner, name));
    if(g != chats.end())
    {
        if(g->second->getMembers().count(user) == 0)
        {
            qDebug() << "User must be a group member";
            return;
        }
        nw->transferGroupRequest(owner, name, user);
    }
}

void Client::deleteGroup(const QString & owner, const QString & name)
{
    if(owner != current_user.get_username())
    {
        qDebug() << "You are not the owner of this group.";
        return;
    }
    auto g = chats.find(groupKey(owner, name));
    if(g != chats.end())
        nw->forwardDeleteGroup(owner, name);
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
