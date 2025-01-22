// file : client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>

namespace oom
{
    class Client : public QObject
    {
        Q_OBJECT

    public:
        Client(int port, QObject * parent=NULL);
        ~Client();
        
    private:
        QTcpSocket * socket;
    };
};

#endif
