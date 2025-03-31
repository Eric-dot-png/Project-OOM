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
        QStringList friends = toQStringList(m["FriendList"].toArray());
        QStringList friendRs = toQStringList(m["FriendRequests"].toArray());
        emit loginValid(user, friends, friendRs);
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
                            m["Messsages"].toString());
    };
    
    emitMap[Protocol::PrivateMessage] = [&](const QJsonObject& m){
        emit detectedPM(m["From"].toString(), m["Message"].toString());
    };
    
    emitMap[Protocol::FriendRequest] = [&](const QJsonObject& m){
        emit detectedFriendReq(m["From"].toString());
    };

    emitMap[Protocol::FriendRemoved] = [&](const QJsonObject& m){
        emit detectedFriendRM(m["From"].toString());
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
