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
#include "User.h"
#include "dbHandler.h"
#include <unordered_map>

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
    Server(int port, QObject * parent=NULL);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    
    //Checks if the string is entirely made up of numbers
    bool numeric(const QString &) const;

    static Server * instance;

    QTimer * timer;
    int port_;
    QTcpServer * listener_;
    dbHandler * db;
    std::unordered_map<QString,
                       std::pair<QHostAddress, quint16>> usermap;
};

#endif
