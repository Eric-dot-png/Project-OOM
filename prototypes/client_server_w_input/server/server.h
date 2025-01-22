// file : server.h
// name : eric garcia

#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>

namespace oom
{
    class Server : public QObject
    {
        Q_OBJECT // mandatory macro???
        
    public:
        // port is the port that the listener will be listening on
        // parent is simply following the convention of the QObject class
        Server(int port, QObject * parent=NULL);

        ~Server();

    private slots:
        // this is the function that is called when
        void onNewConnection();        
    private:
        int port_;
        QTcpServer * listener_;
    };
    
}
#endif
