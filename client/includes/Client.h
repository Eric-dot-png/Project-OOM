// file : Client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <unordered_map>
#include <stdexcept>

#include <QNetworkInterface>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

#include "config.h"
#include "singleton.h"
#include "ProtocolManager.h"
#include "User.h"

class RegMachine; // see regMachine.cpp

class Client : public QObject
{
    Q_OBJECT
        
public:
    Client(Client&) = delete;
    Client& operator=(const Client&) = delete;
    
    static Client * getInstance();
    static void destroyInstance();
    
    //AbstractState * getState() const { return state; }
    User getUser() const { return current_user; }
    User& getUser() { return current_user; }
    // must be in Disconnected state.
    // puts the client into Connecting state
    void connectToServer(const QHostAddress& host, int port);

    // must not be in Disconnected state
    // puts the client into Disconnected state
    void disconnect();

    // sends a message from the client to the server,
    // using protocolmanager's serialize function.
    void writeToServer(Protocol type, const QList<QJsonValue>&);
    
    // must be in Connected state
    // puts the client into loggingin state
    void login(const User &);

    // must be in Connected state
    // puts the client into CreatingAccount state
    void createAccount(const User &);

    // must be in AuthenticatingAccount state
    // puts the client into  Connected State
    void submitAuthCode(const QString&);

    // attempts to discover a user. the idea is to see if a user
    // exists to private message, if they do, their
    // message history is sent as well as their username
    void discover(const User& u);

    void privateMessage(const User& u, const QString& message);
    
    void friendRequest(const User& u);
    
    void acceptFriend(const User& u);

    void extendMessageHistory(const User& u, unsigned int currentSize);

    void getFriendsList(const User& u);
    void getFriendRequestList(const User& u);

signals: 
    void connectedToServer();
    void disconnectedFromServer();
    void loginSuccess();
    void loginFail();
    void accountNotCreated();
    void accountCreated();
    void accountAuthenticated();
    void accountAuthenticationFail();
    
    void discoverUserFail(const QString& username);
    void discoverUserSucceed(const QString& username,
                             const QList<QJsonObject> & messageJsonList);
    
    void recievedDM(const QString& from, const QString& msg);
    void recievedFriendRequest(const QString& from);
    void recievedFriendRemove(const QString& from);

    void sendFriendRequestList(const QString& user, const QStringList& list);
    void sendFriendList(const QString& user, const QStringList& list);

private slots: // these are functions that are connected to signals
    void onReply(); // this function called to handle server info
private:
    Client();
    ~Client();
    
    QHostAddress getDeviceIpAddress(); // returns first non-loopback
                                       // ipv4 address

    class AbstractState;
    
    AbstractState * state;
    QTcpSocket * socket;
    User current_user;
    static Client * instance;
    
    /*
      ClientState Objects.
      These Objects are used to follow the State Design pattern.
      Basically, handle messages differently depending on the current state.
      Each object has a handle() function that handles the message recieved
      from the server.
    */
    class AbstractState
    {
    public:
        virtual void handle(const QJsonObject & m) = 0;
    protected:
        AbstractState() {}
        virtual ~AbstractState() {};
        void StateFailure(const QJsonObject & m);
    };
    
    class DisconnectedState: public AbstractState,
                             public Singleton<DisconnectedState>
    {
        friend class Singleton<DisconnectedState>;
    private:
        DisconnectedState() = default;
        ~DisconnectedState() = default;
    public:
        DisconnectedState& operator=(const DisconnectedState&) = delete;
        DisconnectedState(const DisconnectedState&) = delete;
        void handle(const QJsonObject & m);
    };
    
    class DisconnectingState: public AbstractState,
                              public Singleton<DisconnectingState>
    {
        friend class Singleton<DisconnectingState>;
    private:
        DisconnectingState() = default;
        ~DisconnectingState() = default;
    public:
        DisconnectingState& operator=(const DisconnectingState&) = delete;
        DisconnectingState(const DisconnectingState&) = delete;
        void handle(const QJsonObject & m);
    };
    
    class ConnectingState: public AbstractState,
                           public Singleton<ConnectingState>
    {
        friend class Singleton<ConnectingState>;
    private:
        ConnectingState() = default;
        ~ConnectingState() = default;
    public:
        ConnectingState & operator=(const ConnectingState&) = delete;
        ConnectingState(const ConnectingState&) = delete;
        void handle(const QJsonObject & m);
    };
    
    class ConnectedState: public AbstractState,
                          public Singleton<ConnectedState>
    {
        friend class Singleton<ConnectedState>;
    private:
        ConnectedState() = default;
        ~ConnectedState() = default;
    public:
        ConnectedState & operator=(const ConnectedState&) = delete;
        ConnectedState(const ConnectedState&) = delete;
        void handle(const QJsonObject & m);
    };
    
    class LoggingInState: public AbstractState,
                          public Singleton<LoggingInState>
    {
        friend class Singleton<LoggingInState>;
    private:
        LoggingInState() = default;
        ~LoggingInState() = default;
    public:
        LoggingInState & operator=(const LoggingInState&) = delete;
        LoggingInState(const LoggingInState&) = delete;
        void handle(const QJsonObject & m);
    };

    class LoggedInState: public AbstractState,
                         public Singleton<LoggedInState>
    {
        friend class Singleton<LoggedInState>;
    private:
        LoggedInState() = default;
        ~LoggedInState() = default;
    public:
        LoggedInState & operator=(const LoggedInState&) = delete;
        LoggedInState(const LoggedInState&) = delete;
        void handle(const QJsonObject & m);
    };
    
    class CreatingAccountState: public AbstractState,
                                public Singleton<CreatingAccountState>
    {
        friend class Singleton<CreatingAccountState>;
    private:
        CreatingAccountState() = default;
        ~CreatingAccountState() = default;
    public:
        CreatingAccountState& operator=(const CreatingAccountState&) = delete;
        CreatingAccountState(const CreatingAccountState&) = delete;
        void handle(const QJsonObject & m);
    };
    
    void destroyClientStates();
};

#endif
