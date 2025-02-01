// file : server.h
// name : eric garcia

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>

#include "ProtocolManager.h"
#include "User.h"

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
        bool valid(const QString& usr, const QString& pwd) const;
        
        int port_;
        QTcpServer * listener_;
    };
}

#endif
