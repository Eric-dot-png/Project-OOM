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
#include "ProtocolManager.h"
#include "User.h"
#include "ClientState.h"

class RegMachine;

class Client : public QObject
{
    Q_OBJECT
        
public:

    /*--------------------------------------------
      ClientState Stuff
    --------------------------------------------*/
    
    class AbstractState
    {
    public:
        virtual void handle(const QJsonObject & m) = 0;
    protected:
        AbstractState() {}
        virtual ~AbstractState() {};
        void StateFailure(const QJsonObject & m)
        {
            qDebug() << "Cannot Process " << m["Type"].toInt();
        }
    };
    
    class DisconnectedState: public AbstractState
    {
    public:
        static DisconnectedState * getInstance()
        {
            if(instance == NULL)
                instance = new DisconnectedState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static DisconnectedState * instance;
    };
    
    class DisconnectingState: public AbstractState
    {
    public:
        static DisconnectingState * getInstance()
        {
            if(instance == NULL)
                instance = new DisconnectingState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static DisconnectingState * instance;
    };
    
    class ConnectingState: public AbstractState
    {
    public:
        static ConnectingState * getInstance()
        {
            if(instance == NULL)
                instance = new ConnectingState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static ConnectingState * instance;
    };
    
    class ConnectedState: public AbstractState
    {
    public:
        static ConnectedState * getInstance()
        {
            if(instance == NULL)
                instance = new ConnectedState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static ConnectedState * instance;
    };
    
    class LoggingInState: public AbstractState
    {
    public:
        static LoggingInState * getInstance()
        {
            if(instance == NULL)
                instance = new LoggingInState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static LoggingInState * instance;
    };
    
    class LoggedInState: public AbstractState
    {
    public:
        static LoggedInState * getInstance()
        {
            if(instance == NULL)
                instance = new LoggedInState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static LoggedInState * instance;
    };
    
    class CreatingAccountState: public AbstractState
    {
    public:
        static CreatingAccountState * getInstance()
        {
            if(instance == NULL)
                instance = new CreatingAccountState();
            return instance;
        }
        static void destroyInstance()
        {
            if(instance != NULL)
            {
                delete instance;
                instance = NULL;
            }
        }
        void handle(const QJsonObject & m);
    private:
        static CreatingAccountState * instance;
    };
    
    void destroyClientStates();

    /*--------------------------------------------
      Beginning of Actual Client
    --------------------------------------------*/
    Client(Client&) = delete;
    Client& operator=(const Client&) = delete;
    
    static Client * getInstance();
    static void destroyInstance();
    
    AbstractState * getState() const { return state; }
    User getUser() const { return current_user; }
    
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
    
    void discover(const User& u);
    
    void privateMessage(const User& u, const QString& message);

    void friendRequest(const User& u);

    void acceptFriend(const User& u);

    void extendMessageHistory(const User& u, unsigned int currentSize);

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
private slots: // these are functions that are connected to signals
    void onReply(); // this function called to handle server info
    
private:
    Client(QObject * parent=NULL);
    ~Client();

    QHostAddress getDeviceIpAddress(); // returns first non-loopback
                                       // ipv4 address

    User current_user;
    AbstractState * state;
    static Client * instance;
    QTcpSocket * socket; // comm with server
};

#endif
