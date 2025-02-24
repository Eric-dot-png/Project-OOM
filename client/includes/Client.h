// file : Client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <unordered_map>
#include <stdexcept>

#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

#include "ProtocolManager.h"
#include "User.h"

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

    Client(Client&) = delete;
    Client& operator=(const Client&) = delete;
    
    static Client * getInstance();
    static void destroyInstance();
        
    ClientState getState() const { return state; }
    User getUser() const { return current_user; }
        
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

    // must be in Logged in state
    // sends server the client's ip and port, and allows recieving of
    // messages
    void openListener();

    // must be in Logged in state
    // tells the server that the client no longer listening, and
    // stops recieving message
    void closeListener();

    void discover(const User& u);
    
    void privateMessage(const User& u, const QString& message);
    
signals: // these are signals that trigger effects for the UI
    void connectedToServer();
    void disconnectedFromServer();
    void loginSuccess();
    void accountCreated();
    void accountAuthenticated();
    void accountAuthenticationFail();
    void recievedDM(const QString& from, const QString& msg);
    void discoverUserFail(const QString& username);
    void discoverUserSucceed(const QString& username);
                                                            
private slots: // these are functions that are connected to signals
    void onReply(); // this function called to handle server info
    void onDM(); // this function called to handle dms from other users
    
private:
    Client(QObject * parent=NULL);
    ~Client();
        
    void sendDataToOtherClient(const QHostAddress& ip,
                               const quint16& port,
                               const QByteArray & data) const;
        
        
    /************************************************************
           The following functions handle the client's state.
           They are called inside of onReply()
    *************************************************************/
    void handleLoggedInState(const QJsonObject& m);
    void handleLoggingInState(const QJsonObject& m);
    void handleCreatingAccountState(const QJsonObject& m);
    void handleAuthenticatingAccountState(const QJsonObject& m);
    /************************************************************/

    
    
    static Client * instance;
    ClientState state;
    QTcpSocket * socket; // comm with server
    User current_user;
    QTcpServer * listener; // recieve direct messages 
    QTcpSocket * sender; // send direct messages
        
    std::unordered_map<QString,
                       std::pair<QHostAddress, quint16>> usermap;

};

#endif
