// file : server.h
// name : eric garcia

#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <string>
#include <unordered_map>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "ProtocolManager.h"
#include "dbHandler.h"
#include "config.h"
#include "TcpBuffer.h"

class Server : public QObject, public Singleton<Server>
{
    Q_OBJECT
    friend class Singleton<Server>;
private slots: // these are functions that are connected to signals
    void update();
    void onNewConnection(); // handles client requests 
private:
    Server(QObject * parent=NULL);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void handleBufferReadyRead(QTcpSocket * socket, const QByteArray& data);
    void writeToUserRaw(const QString& name, const QByteArray& data);
    void writeToSocket(QTcpSocket * s, Protocol t,
                       const QList<QJsonValue>& argv);
    /*----------------------------------------------------------------
      onNewConnection Functions
    ----------------------------------------------------------------*/
    void handleFriendRequest(const QJsonObject &, const QByteArray &);
    void handleFriendAccept(const QJsonObject &, const QByteArray &);
    void handleFriendDenied(const QJsonObject&, const QByteArray&);
    void handleFriendRemoved(QTcpSocket *, const QJsonObject &,
                             const QByteArray &);
    // ^^ !! ^^ NOT IMPLEMENTED ^^ !! ^^
    void handlePrivateMessage(const QJsonObject &,
                              const QByteArray &);
    void handleDiscoveryRequest(QTcpSocket *, const QJsonObject &);
    void handleLoginRequest(QTcpSocket *, const QJsonObject &);
    void handleAuthCodeSubmit(QTcpSocket *, const QJsonObject &);
    void handleCreateAccountRequest(QTcpSocket *, const QJsonObject &);
    void handleExtendMessageHistory(QTcpSocket *, const QJsonObject&);
    void handleCreateGroupRequest(QTcpSocket *, const QJsonObject &,
                                  const QByteArray &);
    void handleGroupMessage(const QJsonObject &, const QByteArray &);
    void handleAnnounceOffline(const QJsonObject&);
    void handleUnblockUser(const QJsonObject&);
    void handleBlockUser(const QJsonObject&); 
    void handleGetGroupHistory(QTcpSocket *, const QJsonObject &);
    void handleLeaveGroup(const QJsonObject &, const QByteArray &);
    void handleAddGroupMember(const QJsonObject &, const QByteArray &);
    
    static Server * instance;

    QTcpServer * listener;
    QTimer * timer;
    dbHandler * db;
    
    std::unordered_map<QString, QTcpSocket*> onlineUserMap;
    std::unordered_map<QTcpSocket*, TcpBuffer> buffers;
};

#endif
