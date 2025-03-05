// file : server.h
// name : eric garcia

#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <string>

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
#include <unordered_map>

#include "config.h"

class Server : public QObject
{
    Q_OBJECT 
public:
    static Server * getInstance();
    static void destroyInstance();
                                 
private slots: // these are functions that are connected to signals
    void update();
    void onNewConnection(); // handles client requests 
private:
    Server(QObject * parent=NULL);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void writeToUserRaw(const QString& name, const QByteArray& data);
    void writeToSocket(QTcpSocket * s, Protocol t,
                       const QList<QJsonValue>& argv);
    
    static Server * instance;

    QTimer * timer;
    QTcpServer * listener;
    dbHandler * db;
    
    std::unordered_map<QString, QTcpSocket*> onlineUserMap;
};

#endif
