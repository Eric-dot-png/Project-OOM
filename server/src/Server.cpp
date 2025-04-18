// file : server.cpp
// name : eric garcia
//
// Description: This file defines the Server class, responsible for handling incoming
//              client connections in a Discord-like Qt application. It listens for new
//              connections on a given IP and port, parses client requests, and interacts
//              with the database (via dbHandler) to process authentication, friend
//              requests, and message operations.  

#include "Server.h"

// Helper function to repeatedly hash a password string. Used for password encryption.
QString pwdHash(const QString & s)
{
    QString newS = s;
    for(int i = 0; i < 10; i++)
        newS = QString::number(qHash(newS));
    return newS;
}

// Utility function to check if all characters in a string are numeric (0-9).
bool numeric(const QString & s) 
{
    for(const QChar & c : s)
        if(c < '0' || c > '9')
            return 0;
    return 1;
}

QJsonArray toJArray(const QStringList& li)
{
    QJsonArray ret;
    
    for (const QString& v : li)
        ret.append(v);
    
    return ret;
}

// Constructor: sets up the TCP server, database handler instance, and a timer used for
// periodic maintenance tasks (e.g., cleaning up registrations).
Server::Server(QObject * parent)
    : QObject(parent), listener(new QTcpServer(this)),
      timer(new QTimer(this)),
      db(dbHandler::GetInstance())
{
    // When a new client connects, call onNewConnection
    connect(listener, &QTcpServer::newConnection, this,
            &Server::onNewConnection);

    // When the timer times out, call the update() function
    connect(timer, &QTimer::timeout, this, &Server::update);
    
    // Start listening on the specified IP and port
    if (listener->listen(SERVER_HOST_IP, SERVER_PORT))
    {
        qDebug() << "Listening on port"
                 << listener->serverPort() << "...";
    }
    else
    {
        qDebug() << "ERROR: Server could not start!";
    }
    
    timer->setInterval(60000); // 1 minute interval
    
    // Start the timer
    timer->start();
}
    
// Destructor: closes the TCP listener when the server object is destroyed.
Server::~Server()
{
    listener->close();
}
    
// Called periodically to perform maintenance tasks, such as cleaning up registrations.
void Server::update()
{
    db->cleanReg();
    timer->start();
}

// Called when a new client connects. Retrieves the pending connection socket and sets up
// a lambda to read incoming data whenever it's available.
void Server::onNewConnection()
{
    QTcpSocket * client = listener->nextPendingConnection();
    
    // Set up a lambda function to handle incoming data from this client
    connect(client, &QTcpSocket::readyRead, this, [this, client]()
    {
        QByteArray data = client->readAll();
        qDebug() << "Recieved" << data << "from"
                 << client->socketDescriptor();

        if (buffers.find(client) == buffers.end())
        {
            qDebug() << "Buffer not found, initializing new buffer...";
            buffers[client] = TcpBuffer();
            qDebug() << "Done.";
        }
        qDebug() << "Pushing data to buffer...";
        buffers[client].append(data);
        qDebug() << "Done.";
        if (buffers[client].readyRead())
        {
            qDebug() << "Buffer is ready to be read. Reading...";
            // note : buffer.read() clears the buffer
            handleBufferReadyRead(client, buffers[client].read());
            qDebug() << "Done.";
        }
    });
}

void Server::handleBufferReadyRead(QTcpSocket * client,const QByteArray& data)
{
    // Deserialize the incoming JSON data into a QJsonObject
        QJsonObject m = ProtocolManager::deserialize(data);
        
        Protocol type = static_cast<Protocol>(m["Type"].toInt());
        
        // Check the protocol type to decide which operation to perform
        switch(type)
        {
            case Protocol::UnblockUser:
            {
                handleUnblockUser(m);
                break;
            }
            case Protocol::BlockUser:
            {
                handleBlockUser(m);
                break;
            }
            case Protocol::AnnounceOffline:
            {
                handleAnnounceOffline(m);
                break;
            }
            case Protocol::ExtendMessageHistory:
            {
                handleExtendMessageHistory(client, m);
                break;
            }
            case Protocol::FriendRequest:
            {
                handleFriendRequest(m, data);
                break;
            }
            case Protocol::FriendAccept:
            {
                handleFriendAccept(m, data);
                break;
            }
            case Protocol::FriendDenied:
            {
                handleFriendDenied(m, data);
                break;
            }
            case Protocol::FriendRemoved:
            {
                handleFriendRemoved(client, m, data);
                break;
            }
            case Protocol::PrivateMessage:
            {
                handlePrivateMessage(m, data);
                break;
            }
            case Protocol::DiscoveryRequest:
            {
                handleDiscoveryRequest(client, m);
                break;
            }
            case Protocol::LoginRequest:
            {
                handleLoginRequest(client, m);
                break;
            }
            case Protocol::AuthCodeSubmit:
            {
                handleAuthCodeSubmit(client, m);
                break;
            }
            case Protocol::CreateAccountRequest:
            {
                handleCreateAccountRequest(client, m);
                break;
            }
            case Protocol::CreateGroupRequest:
            {
                handleCreateGroupRequest(client, m, data);
                break;
            }
            case Protocol::GroupMessage:
            {
                handleGroupMessage(m, data);
                break;
            }
            case Protocol::GetGroupHistory:
            {
                handleGetGroupHistory(client, m);
                break;
            }
            case Protocol::LeaveGroup:
            {
                handleLeaveGroup(m, data);
                break;
            }
            case Protocol::AddGroupMember:
            {
                handleAddGroupMember(m, data);
                break;
            }
            case Protocol::TransferGroup:
            {
                handleTransferGroup(m, data);
                break;
            }
            case Protocol::DeleteGroup:
            {
                handleDeleteGroup(m, data);
                break;
            }
            default:
            {
                // Handle unexpected protocol types
                qDebug() << "Unknown request... ";
                break;
            }
        }
}

// Writes a raw QByteArray message directly to a user if they're in the onlineUserMap.
void Server::writeToUserRaw(const QString& to, const QByteArray& data)
{
    if (onlineUserMap.find(to) != onlineUserMap.end() &&
        onlineUserMap.find(to)->second != NULL)
    {
        TcpSmartWrite(onlineUserMap[to], data);
    }
}

// Serializes protocol data into JSON and writes it to the given socket.
void Server::writeToSocket(QTcpSocket * socket, Protocol type,
                           const QList<QJsonValue>& argv)
{
    if (socket != NULL)
    {
        TcpSmartWrite(socket, ProtocolManager::serialize(type,argv));
    }
}

/*--------------------------------------------------------------------
  Logic for onNewConnection
--------------------------------------------------------------------*/
void Server::handleFriendRequest(const QJsonObject & m,
                                 const QByteArray & data)
{
    // Insert a friend request into the database, then forward the message
    // to the recipient if they're online.
    db->addFriendRequest(m["From"].toString(), m["To"].toString());
    writeToUserRaw(m["To"].toString(), data);
}

void Server::handleFriendAccept(const QJsonObject & m,
                                const QByteArray & data)
{
    // Finalize friendship, then notify the recipient if they're online.
    db->addFriend(m["From"].toString(), m["To"].toString());
    writeToUserRaw(m["To"].toString(), data);
}

void Server::handleFriendDenied(const QJsonObject& m, const QByteArray& data)
{
    db->removeFriendRequest(m["From"].toString(), m["To"].toString());
    writeToUserRaw(m["To"].toString(), data);
}

void Server::handleFriendRemoved(QTcpSocket * client, const QJsonObject & m,
                                 const QByteArray & data)
{
    db->removeFriend(m["From"].toString(), m["To"].toString());
    writeToUserRaw(m["To"].toString(), data);
}

void Server::handlePrivateMessage(const QJsonObject & m,
                                  const QByteArray & data)
{
    // Store the private message in the database, then pass it to the recipient.
    db->storeMessage(m);
    writeToUserRaw(m["To"].toString(), data);
}

void Server::handleDiscoveryRequest(QTcpSocket * client,
                                    const QJsonObject & m)
{
    // Check if the requested user exists, then retrieve or fail to retrieve
    // the message history.
    QString usrname = m["Username"].toString();
    bool existing_user = db->userExists(usrname);
    if (existing_user)
    {
        QJsonArray msgHist = db->getMessages(m["CurrUser"].toString(),
                                             usrname);
        writeToSocket(client, Protocol::DiscoveryAccept,
                      {usrname, msgHist});
    }
    else
    {
        writeToSocket(client, Protocol::DiscoveryFail, {usrname});
    }
}

void Server::handleLoginRequest(QTcpSocket * client, const QJsonObject & m)
{
    // Validate the user's login credentials via the database
    QString usr = m["Username"].toString();
    QString pwd = pwdHash(m["Password"].toString());
    
    if (db->loginValidate(usr, pwd))
    {
        // If valid, notify the client and mark the user as online
        QJsonArray fs = toJArray(db->getFriendslist(usr));
        QJsonArray frs = toJArray(db->getFriendRequests(usr));
        QJsonArray groups = db->getGroups(usr);
        QJsonArray blocklist = toJArray(db->getBlockList(usr));
        writeToSocket(client, Protocol::LoginAccept,
                      {usr, fs, frs, groups, blocklist});
        
        onlineUserMap[usr] = client;
    }
    else
    {
        // If invalid, send a login failure message to the client
        writeToSocket(client, Protocol::LoginDenied,
                      {"Account with login information DNE"});
    }
}
void Server::handleAuthCodeSubmit(QTcpSocket * client, const QJsonObject & m)
{
    // Validate a user's registration code for email verification
    QString usr  = m["Username"].toString();
    QString code = m["Code"].toString();
    
    // If code is non-numeric or not 6 digits long, fail immediately
    if (!numeric(code) || code.length() != 6)
    {
        writeToSocket(client, Protocol::AuthCodeDenied,
                      {"Invalid Code"});
    }
    else
    {
        // Check database for match; if success, accept; otherwise deny
        bool success = db->emailValidate(usr, code);
        if (success)
            writeToSocket(client, Protocol::AuthCodeAccept, {});
        else
            writeToSocket(client, Protocol::AuthCodeDenied,
                          {"Unknown Error, Could not authenticate account."});
    }
}

void Server::handleCreateAccountRequest(QTcpSocket * client,
                                        const QJsonObject & m)
{
    // Create a new account in the database, then optionally send an email
    // with the validation code if not auto-validated.
    QString usr   = m["Username"].toString();
    QString pwd   = pwdHash(m["Password"].toString());
    QString email = m["Email"].toString();
    
    // Ensure username doesn't already exist
    if (!db->availUsername(usr))
    {
        writeToSocket(client, Protocol::CreateAccountDenied,
                      {"Username already exists."});
    }
    else
    {
        // If newUser() returns a code, it's pending validation
        QString code = db->newUser(usr, pwd, email);
        if (code != "")
        {
            // Attempt to send a validation email
            std::string emailsyscall = "python3 src/myemail.py "
                + email.toStdString() + ' ' + code.toStdString();
            int emailStatus = std::system(emailsyscall.c_str());
            
            if (emailStatus == 0)
            {
                // If success, notify the client
                writeToSocket(client, Protocol::CreateAccountAccept,
                              {usr, pwd});
            }
            else
            {
                // If email sending fails, remove registration record
                // and inform the client
                db->removeReg(usr);
                writeToSocket(client, Protocol::CreateAccountDenied,
                              {"Could not send email."});
            }
        }
        else
        {
            // If newUser() returns an empty string, account creation failed
            writeToSocket(client, Protocol::CreateAccountDenied,
                          {"Unknown Error, Could not create account."});
        }
    }
}

void Server::handleExtendMessageHistory(QTcpSocket * client,
                                        const QJsonObject & m)
{
    QString usrname = m["Username"].toString();
    bool existing_user = db->userExists(usrname);
    if (existing_user)
    {
        QString requester = m["CurrUser"].toString();
        qint64 start = m["Start"].toInt();

        QJsonArray msgs = db->getMessages(requester, usrname, start);
        writeToSocket(client, Protocol::ExtendMessageHistoryAccept,
                      {usrname, msgs});
    }
    else
    {
        writeToSocket(client, Protocol::ExtendMessageHistoryDenied,{usrname});
    }
}

void Server::handleCreateGroupRequest(QTcpSocket * client,
                                      const QJsonObject & m,
                                      const QByteArray & data)
{
    QString owner = m["Owner"].toString();
    QString name = m["Name"].toString();
    bool avail = db->availGroup(owner, name);
    if(avail)
    {
        if(m["Members"].isArray())
        {
            QStringList members;
            QJsonArray marray = m["Members"].toArray();
            for(const QJsonValue & mem : marray)
                members.append(mem.toString());
            bool success = db->newGroup(owner, name, members);
            if(success)
            {
                writeToSocket(client, Protocol::CreateGroupAccept,
                              {owner, name, marray});
                for(const QString & mem : members)
                    writeToUserRaw(mem, data);
            }
            else
                writeToSocket(client, Protocol::CreateGroupFail,
                              {owner, name, "Couldn't create group"});
        }
        else
            writeToSocket(client, Protocol::CreateGroupFail,
                          {owner, name, "invalid member format"});
    }
    else
        writeToSocket(client, Protocol::CreateGroupFail,
                      {owner, name, "Group name already used"});
}

void Server::handleGroupMessage(const QJsonObject & m,
                                const QByteArray & data)
{
    // Store the private message in the database, then pass it to the recipient.
    qDebug() << "Storing group message...";
    bool success = db->storeGroupMessage(m);
    if(!success)
        return;
    qDebug() << "Getting members...";
    QStringList members = db->getGroupMembers(m["Owner"].toString(),
                                              m["Name"].toString());
    qDebug() << "Forwarding message to members...";
    for(const QString & member : members)
    {
        if(member != m["From"].toString())
            writeToUserRaw(member, data);
    }
}

void Server::handleAnnounceOffline(const QJsonObject& m)
{
    QString u = m["Username"].toString();
    auto p = onlineUserMap.find(u);
    if (p != onlineUserMap.end())
        onlineUserMap.erase(p);
}

void Server::handleUnblockUser(const QJsonObject& m)
{
    db->removeBlock(m["From"].toString(), m["To"].toString());
}

void Server::handleBlockUser(const QJsonObject& m)
{
    db->addBlock(m["From"].toString(), m["To"].toString());
}

void Server::handleGetGroupHistory(QTcpSocket * client, const QJsonObject & m)
{
    QJsonArray messages = db->getGroupMessages(m["Owner"].toString(),
                                               m["Name"].toString(),
                                               m["CurrSize"].toInt());
    writeToSocket(client, Protocol::GetGroupHistorySuccess,
                  {m["Owner"].toString(), m["Name"].toString(), messages});
}

void Server::handleLeaveGroup(const QJsonObject & m, const QByteArray & data)
{
    int groupId = db->getGroupId(m["Owner"].toString(), m["Name"].toString());
    db->removeGroupMember(groupId, m["User"].toString());
    QStringList members = db->getGroupMembers(groupId);
    for(QString & mem : members)
        writeToUserRaw(mem, data);
}

void Server::handleAddGroupMember(const QJsonObject & m,
                                  const QByteArray & data)
{
    QStringList members = db->getGroupMembers(m["Owner"].toString(),
                                              m["Name"].toString());
    db->addGroupMember(m["Owner"].toString(), m["Name"].toString(),
                       m["User"].toString());
    auto pair = onlineUserMap.find(m["Name"].toString());
    if(pair != onlineUserMap.end())
        writeToSocket(pair->second, Protocol::CreateGroupRequest,
                      {m["Owner"].toString(), m["Name"].toString(),
                       QJsonArray::fromStringList(members)});
    for(QString mem : members)
        writeToUserRaw(mem, data);
}

void Server::handleTransferGroup(const QJsonObject & m,
                                 const QByteArray & data)
{
    if(!db->changeGroupOwner(m["Owner"].toString(), m["Name"].toString(),
                             m["User"].toString()))
        return;
    QStringList members = db->getGroupMembers(m["User"].toString(),
                                              m["Name"].toString());
    for(QString & mem : members)
        writeToUserRaw(mem, data);
}

void Server::handleDeleteGroup(const QJsonObject & m,
                               const QByteArray & data)
{
    QStringList members = db->getGroupMembers(m["Owner"].toString(),
                                              m["Name"].toString());
    if(db->deleteGroup(m["Owner"].toString(), m["Name"].toString()))
    {
        for(QString & mem : members)
            writeToUserRaw(mem, data);
    }
}
