// file : Client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QSocketNotifier>
#include <QObject>
#include <QDebug>

#include "ProtocolManager.h"

namespace oom
{
    class Client : public QObject
    {
        Q_OBJECT
        
    public:
        enum ClientState {
            Disconnected=0,
            Disconnecting,
            Connecting,
            Connected,
            LoggingIn,
            LoggedIn
        };
        
        Client(QObject * parent=NULL);
        ~Client();
        
        void connectToServer(const QHostAddress& host, int port);
        void disconnect();

        void login(const QString& usr, const QString& pwd);
        
        ClientState getState() const { return state; }
    private slots:
        void onReply();
        
    private:
        ClientState state;
        QTcpSocket * socket;
    };
};

#endif
