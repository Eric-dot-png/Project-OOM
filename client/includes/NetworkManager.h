#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <functional>
#include <unordered_map>

#include <QObject>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "config.h"
#include "ProtocolManager.h"
#include "singleton.h"

/*
  This class handles network communications with the server.
 */
class NetworkManager : public QObject, public Singleton<NetworkManager>
{
    Q_OBJECT
    friend class Singleton<NetworkManager>;
public:
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    // Connect Network Manager to the server
    void connect();

    // Disconnect Network Manager from server
    void disconnect();
    
    void validateLogin(const QString& usr, const QString& pwd) const;
    
    void setupAcc(const QString& usr, const QString& pwd,
                  const QString& eml) const;
    
    void authAcc(const QString& usr, const QString & pwd,
                    const QString& code) const;
    
    void pmHistory(const QString& usr1, const QString& usr2) const;
    
    void forwardPM(const QString& to, const QString& from,
                   const QString& content) const;
    
    void forwardFriendReq(const QString& to, const QString& from) const;

    void forwardFriendAccept(const QString& to, const QString& from) const;

    void forwardFriendDeny(const QString& to, const QString& from) const;

    void forwardFriendRemove(const QString& to, const QString& from) const;

    void requestMoreMessages(const QString& to, const QString& from,
                             const qint64 current_amount) const; 
signals:
    void connected();
    void disconnected();
    void loginValid(const QString& usr, const QStringList& friends,
                    const QStringList& friendRs);
    void loginInvalid();
    void accSetupPass(const QString& usr, const QString& pwd);
    void accSetupFail();
    void accAuthPass();
    void accAuthFail();
    void userDNE(const QString& user);
    void pmHistoryFound(const QString& user, const QString& messages);
    void detectedPM(const QString& from, const QString& message);
    void detectedFriendReq(const QString& from);
    void detectedFriendRM(const QString& from);
    void moreMessages(const QString& user, const QString& messages);
    void failedMoreMessages(const QString& user);
private:
    QTcpSocket * socket;
    std::unordered_map<Protocol,std::function<void(QJsonObject)>> emitMap;
    
    NetworkManager();
    ~NetworkManager();
    
    // utility function to write to the server. uses protocol manager
    // to serialize the arguments.
    void writeToServer(Protocol type, const QList< QJsonValue > & argv) const;
                                                                             
private slots:

    // Handles messages recieved from the server. Emits signals
    // based on the message type
    void handleServerMessage();
};

#endif  
