#include "NetworkManager.h"

NetworkManager::NetworkManager()
    : socket(new QTcpSocket(this)),
      emitMap({})
{
    // Socket connected? Then emmit connected() signal
    QObject::connect(socket, &QTcpSocket::connected, this, [&](){
        qDebug() << "NetworkManager Connected to Server";
        emit connected();
    });
    
    // likewise, for disconnected
    QObject::connect(socket, &QTcpSocket::disconnected, this, [&](){
        qDebug() << "NetworkManager Disconnected from Server";
        emit disconnected();
    });

    // When the socket recievess a message, call handleServerMessage()
    QObject::connect(socket, &QTcpSocket::readyRead, this,
            &NetworkManager::handleServerMessage);

    // Initialize emitMap
    emitMap[Protocol::LoginAccept] = [&](const QJsonObject& m){
        auto toQStringList = [](const QJsonArray& arr)
        {
            QStringList ret;
            for (const auto& v : arr)
                ret.append(v.toString());
            return ret;
        };
        
        QString user = m["Username"].toString();
        QStringList friends = toQStringList(m["FriendsList"].toArray());
        QStringList friendRs =toQStringList(m["FriendRequestList"].toArray());
        QJsonArray groups = m["Groups"].toArray();
        emit loginValid(user, friends, friendRs, groups);
    };
    
    emitMap[Protocol::LoginDenied] = [&](const QJsonObject& m){
        emit loginInvalid();
    };

    emitMap[Protocol::CreateAccountAccept] = [&](const QJsonObject& m){
        emit accSetupPass(m["Username"].toString(), m["Password"].toString());
    };

    emitMap[Protocol::CreateAccountDenied] = [&](const QJsonObject& m){
        emit accSetupFail();
    };
    
    emitMap[Protocol::AuthCodeAccept] = [&](const QJsonObject& m){
        emit accAuthPass();
    };

    emitMap[Protocol::AuthCodeDenied] = [&](const QJsonObject& m){
        emit accAuthFail();
    };
    
    emitMap[Protocol::DiscoveryFail] = [&](const QJsonObject& m){
        emit userDNE(m["Username"].toString());
    };
    
    emitMap[Protocol::DiscoveryAccept] = [&](const QJsonObject& m){
        emit pmHistoryFound(m["Username"].toString(),
                            m["Messages"].toArray());
    };
    
    emitMap[Protocol::PrivateMessage] = [&](const QJsonObject& m){
        emit detectedPM(m["From"].toString(), m["Message"].toString());
    };
    
    emitMap[Protocol::FriendRequest] = [&](const QJsonObject& m){
        emit detectedFriendReq(m["From"].toString());
    };

    emitMap[Protocol::FriendAccept] = [&](const QJsonObject& m){
        emit detectedFriendAccept(m["From"].toString());
    };
    
    emitMap[Protocol::FriendDenied] = [&](const QJsonObject& m){
        emit detectedFriendDeny(m["From"].toString());
    };
    
    emitMap[Protocol::FriendRemoved] = [&](const QJsonObject& m){
        emit detectedFriendRM(m["From"].toString());
    };

    emitMap[Protocol::ExtendMessageHistoryAccept] = [&](const QJsonObject& m){
        emit moreMessages(m["Username"].toString(), m["Messages"].toArray());
    };

    emitMap[Protocol::ExtendMessageHistoryDenied] = [&](const QJsonObject& m){
        emit failedMoreMessages(m["Username"].toString());
    };

    emitMap[Protocol::CreateGroupAccept] = [&](const QJsonObject& m){
        QJsonArray memArray = m["Members"].toArray();
        QStringList members;
        for(const auto & member : memArray)
            members.append(member.toString());
        emit createGroupPass(m["GroupName"].toString(), members);
    };
    
    emitMap[Protocol::CreateGroupFail] = [&](const QJsonObject& m){
        emit createGroupFail(m["Message"].toString());
    };
}

NetworkManager::~NetworkManager()
{
    emit disconnected();
}

void NetworkManager::connect()
{
    qDebug() << "Network Manager : called connect";
    if (socket->state() != QAbstractSocket::ConnectedState)
        socket->connectToHost(SERVER_IP, SERVER_PORT); // See config.h
}

void NetworkManager::disconnect()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void NetworkManager::validateLogin(const QString& usr, const QString& pwd)
    const
{
    writeToServer(Protocol::LoginRequest, {usr, pwd});
}

void NetworkManager::setupAcc(const QString& usr, const QString& pwd,
                              const QString& eml) const
{
    writeToServer(Protocol::CreateAccountRequest, {usr, pwd, eml});
}

void NetworkManager::authAcc(const QString& usr, const QString & pwd,
                             const QString& code) const
{
    writeToServer(Protocol::AuthCodeSubmit, {usr, pwd, code});
}

void NetworkManager::pmHistory(const QString& usr1, const QString& usr2)
    const
{
    writeToServer(Protocol::DiscoveryRequest, {usr1, usr2});
}

void NetworkManager::forwardPM(const QString& to, const QString& from,
               const QString& content) const
{
    writeToServer(Protocol::PrivateMessage, {to, from, content});
}

void NetworkManager::forwardFriendReq(const QString& to, const QString& from)
    const
{
    writeToServer(Protocol::FriendRequest, {from, to});
}


void NetworkManager::forwardFriendAccept(const QString& to,
                                         const QString& from) const
{
    writeToServer(Protocol::FriendAccept, {from, to});
}

void NetworkManager::forwardFriendDeny(const QString& to, const QString& from)
    const
{
    writeToServer(Protocol::FriendDenied, {from, to});
}

void NetworkManager::forwardFriendRemove(const QString& to,
                                         const QString& from) const
{
    writeToServer(Protocol::FriendRemoved, {from, to});
}

void NetworkManager::requestMoreMessages(const QString& to,
                                         const QString& from,
                                         const qint64 current_amount) const
{
    writeToServer(Protocol::ExtendMessageHistory, {from,to,current_amount});
}

void NetworkManager::forwardCreateGroup(const QString & owner,
                                        const QString & name,
                                        const QStringList & members) const
{
    QJsonArray marray = QJsonArray::fromStringList(members);
    writeToServer(Protocol::CreateGroupRequest, {owner, name, marray});
}

void NetworkManager::forwardGroupMessage(const QString & owner,
                                         const QString & name,
                                         const QString & from,
                                         const QString & message) const
{
    writeToServer(Protocol::GroupMessage, {owner, name, from, message});
}
void NetworkManager::writeToServer(Protocol type,
                                   const QList<QJsonValue> & argv) const
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->write(ProtocolManager::serialize(type,argv));
    else qDebug() << "ERROR: Network Manager is not connected to server.";
}

void NetworkManager::handleServerMessage()
{
    QByteArray data = socket->readAll();
    qDebug() << "Network Manager Recieved" << data << "from the server.";
    QJsonObject m = ProtocolManager::deserialize(data);
    Protocol type = static_cast<Protocol>(m["Type"].toInt());
    if (emitMap.find(type) != emitMap.end()) emitMap[type](m);
    else qDebug() << "ERROR: Network Manager cannot handle this data.";
}
