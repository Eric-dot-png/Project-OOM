// file : Client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
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
            LoggedIn,
            CreatingAccount
        };
        
        Client(QObject * parent=NULL);
        ~Client();

        ClientState getState() const { return state; }
        
        void connectToServer(const QHostAddress& host, int port);
        void disconnect();
        
        void login(const QString& usr, const QString& pwd);
<<<<<<< HEAD:prototypes/client_server_dms/client/Client.h
        void createAccount(const QString& usr, const QString& pwd,
                           const QString& email);
=======
        void createAccount(const QString& usr, const QString& pwd);
>>>>>>> main:src/client/Client.h
        
    private slots:
        void onReply();
        
    private:
        ClientState state;
        QTcpSocket * socket;
    };
};

#endif
