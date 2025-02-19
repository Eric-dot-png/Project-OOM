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
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "ProtocolManager.h"
#include "User.h"
#include "dbHandler.h"
#include <unordered_map>

namespace oom
{
    class Server : public QObject
    {
        Q_OBJECT 
    public:
        static Server * getInstance();
        static void destroyInstance();
            
    private slots: // these are functions that are connected to signals
        void onNewConnection(); // handles client requests 
    private:
            Server(int port, QObject * parent=NULL);
        ~Server();
    
        //Checks if the string is entirely made up of numbers
        bool numeric(const QString &) const;
        
        static Server * instance;
        
        int port_;
        QTcpServer * listener_;
        dbHandler * db;
        std::unordered_map<QString,
                           std::pair<QHostAddress, quint16>> usermap;
    };
}

#endif
