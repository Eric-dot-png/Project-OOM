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

namespace oom
{
    class Server : public QObject
    {
        Q_OBJECT 
    public:
        Server(int port, QObject * parent=NULL);
        ~Server();
                 
    private slots:
        void onNewConnection();        
    private:
        //Checks if the string is entirely made up of numbers
        bool numeric(const QString &) const;
        
        int port_;
        QTcpServer * listener_;
        dbHandler db;
    };
}

#endif
