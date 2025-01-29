// file : server.h
// name : eric garcia

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>

// adhawk stuff for db
#include <fstream>
#include <string>
<<<<<<< HEAD:prototypes/client_server_dms/server/Server.h
//
=======
>>>>>>> main:src/server/Server.h

#include "ProtocolManager.h"

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
        bool dbContains(const QString& usr, const QString& pwd) const;
<<<<<<< HEAD:prototypes/client_server_dms/server/Server.h
        void createAccount(const QString& usr, const QString& pwd,
                           const QString& email);
=======
        void createAccount(const QString& usr, const QString& pwd);
>>>>>>> main:src/server/Server.h
        
        int port_;
        QTcpServer * listener_;

    };
}

#endif
