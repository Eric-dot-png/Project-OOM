// file : Client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

#include "ProtocolManager.h"
#include "User.h"

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
            CreatingAccount,
            AuthenticatingAccount
        };
        
        Client(QObject * parent=NULL);
        ~Client();

        ClientState getState() const { return state; }

        // must be in Disconnected state.
        // puts the client into Connecting state
        void connectToServer(const QHostAddress& host, int port);

        // must not be in Disconnected state
        // puts the client into Disconnected state
        void disconnect();

        // must be in Connected state
        // puts the client into loggingin state
        void login(const User &);

        // must be in Connected state
        // puts the client into CreatingAccount state
        void createAccount(const User &);

        // must be in AuthenticatingAccount state
        // puts the client into  Connected State
        void submitAuthCode(const QString&);
        
    signals: // these are signals that trigger effects for the UI
        void connectedToServer();
        void disconnectedFromServer();
        void loginSuccess();
        void accountCreated();
        void accountAuthenticated();
        void accountAuthenticationFail();
                                        
    private slots: // these are functions that are connected to signals
        void onReply(); // this function called to handle server info
        
    private:
        /************************************************************
           The following functions handle the client's state.
           They are called inside of onReply()
        *************************************************************/
        void handleLoginState(const QJsonObject& m);
        void handleCreatingAccountState(const QJsonObject& m);
        void handleAuthenticatingAccountState(const QJsonObject& m);
        /************************************************************/
        
        ClientState state;
        QTcpSocket * socket;
        User current_user;
    };
};

#endif
