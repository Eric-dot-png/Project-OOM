// file : Client.cpp
// name : eric garcia
#include "Client.h"

namespace oom
{
    Client * Client::instance(NULL);
    
    Client::Client(QObject * parent)
        : QObject(parent), state(ClientState::Disconnected),
          socket(new QTcpSocket(this)),
          current_user("None","None","None"), // for now this is guest user
          listener(NULL)
    {
        connect(socket, &QTcpSocket::connected, this, [&](){
            qDebug() << "Connected to Server";
            state = ClientState::Connected;
            emit connectedToServer();
        });
        
        connect(socket, &QTcpSocket::disconnected, this, [&](){
            qDebug() << "Disconnected from Server";
            state = ClientState::Disconnected;
            emit disconnectedFromServer();
        });
        
        connect(socket, &QTcpSocket::readyRead, this, &Client::onReply);
    }
    
    Client::~Client()
    {
        if (state != ClientState::Disconnected)
            disconnect();
        
        delete socket;
    }

    Client * Client::getInstance()
    {
        if (instance == NULL)
            instance = new Client();
        return instance;
    }
    
    void Client::destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    
    void Client::connectToServer(const QHostAddress& host, int port)
    {
        if (state == ClientState::Disconnected)
        {
            qDebug() << "Attempting to connect to server...";
            socket->connectToHost(host,port);
            state = ClientState::Connecting;
        }
        else
        {
            qDebug() << "Already connected to server.";
        }
    }
    
    void Client::disconnect()
    {
        if (state != ClientState::Disconnected)
        {
            qDebug() << "Disconnecting from server...";
            if (state == ClientState::LoggedIn)
            {
                closeListener();
            }
            
            socket->disconnectFromHost();
            state = Disconnecting;
        }
        else
        {
            qDebug() << "Already disconnected.";
        }
    }
    
    void Client::login(const User & u)
    {
        if (state == ClientState::Connected)
        {
            qDebug() << "Attempting Login...";
            socket->write(ProtocolManager::serialize(
                              ProtocolManager::LoginRequest,
                              {u.get_username(), u.get_password()})
                );
            state = ClientState::LoggingIn;
        }
        else
        {
            qDebug() << "Can't Login. Current State:" << state;
        }
    }
    
    void Client::createAccount(const User & u)
    {
        if (state == ClientState::Connected)
        {
            qDebug() << "Attempting Create Account...";
            socket->write(ProtocolManager::serialize(
                              ProtocolManager::CreateAccountRequest,
                              {u.get_username(), u.get_password(),
                               u.get_email()})
                );
            state = ClientState::CreatingAccount;
        }
        else
        {
            qDebug() << "Can't Create Account. Current State:" << state;
        }
    }
    
    void Client::submitAuthCode(const QString& code)
    {
        if (state == ClientState::AuthenticatingAccount)
        {
            qDebug() << "Attempting Authentication...";
            socket->write(ProtocolManager::serialize(
                              ProtocolManager::CreateAccountAuthCodeSubmit,
                              {current_user.get_username(),
                               current_user.get_password(), code})
                );
        }
        else
        {
            qDebug() << "Can't Authenticate Account. Current State:" << state;
        }
    }

    void Client::openListener()
    {
        if (state == ClientState::LoggedIn && listener == NULL)
        {
            listener = new QTcpServer(this);
            if (listener->listen(QHostAddress::LocalHost, 0))
            {
                qDebug() << "Announcing Ip and Port:"
                         << listener->serverAddress().toString()
                         << ","
                         << listener->serverPort();
                
                socket->write(
                    ProtocolManager::serialize(
                        ProtocolManager::AnnounceIpPort, {
                            current_user.get_username(),
                            listener->serverAddress().toString(),
                            QString::number(listener->serverPort()) }
                        )
                    );
            }
            else qDebug() << "ERROR: Listener could not start!";
        }
        else if (listener != NULL)
        {
            qDebug() << "Already announced ip and port.";
        }
        else
        {
            qDebug() << "Can't Announce Ip and port. Current state:"
                     << state; 
        }
    }

    void Client::closeListener()
    {
        if (state == ClientState::LoggedIn && listener != NULL)
        {
            qDebug() << "Closing Listener...";
            delete listener;
            listener = NULL;

            socket->write(
                ProtocolManager::serialize(
                    ProtocolManager::AnnounceOffline, {
                        current_user.get_username() }
                    )
                );
        }
        else if (listener == NULL)
        {
            qDebug() << "Listener already closed.";
        }
        else
        {
            qDebug() << "Can't close listener. Current State:"
                     << state;
        }
    }

    
    void Client::onReply()
    {
        QByteArray data = socket->readAll();
        qDebug() << "Recieved" << data << "from server.";
        
        QJsonObject m = ProtocolManager::deserialize(data);
        
        switch(state)
        {
            case ClientState::LoggingIn:
            {
                handleLoginState(m);
                break;
            }
            case ClientState::CreatingAccount:
            {
                handleCreatingAccountState(m);
                break;
            }
            case ClientState::AuthenticatingAccount:
            {
                handleAuthenticatingAccountState(m);
                break;
            }
            default:
            {
                qDebug() << "State Failure:" << "\nCurrentState: " << state;
                break;
            }
        }
    }

    void Client::handleLoginState(const QJsonObject& m)
    {
        switch(m["Type"].toInt())
        {
            case ProtocolManager::LoginAccept:
            {
                qDebug() << "Login Success!";
                state = ClientState::LoggedIn;
                User u(m["Username"].toString(),
                       m["Password"].toString());
                current_user = u;
                openListener();
                qDebug() << "Username:" << u.get_username();
                emit loginSuccess();
                break;
            }
            case ProtocolManager::LoginDenied:
            {
                qDebug() << "Login Failed!";
                state = ClientState::Connected;
                break;
            }
            default:
            {
                qDebug() << "State Transition Failure:"
                         << "\nCurrent State:" << state
                         << "\n  Attempting transition to:"
                         << m["Type"].toInt();
                state = ClientState::Connected;
                break;
            }
        }
    }

    void Client::handleCreatingAccountState(const QJsonObject& m)
    {
        switch(m["Type"].toInt())
        {
            case ProtocolManager::CreateAccountAccept:
            {
                User u(m["Username"].toString(),
                       m["Password"].toString());
                qDebug() << "Account with username"
                         << u.get_username()
                         << "created!";
                current_user = u;
                emit accountCreated();
                state = ClientState::AuthenticatingAccount;
                break;
            }
            case ProtocolManager::CreateAccountDenied:
            {
                qDebug() << "Account Creation Failed!";
                state = ClientState::Connected;
                break;
            }
            default:
            {
                qDebug() << "State Transition Failure:"
                         << "\nCurrent State:" << state
                         << "\n  Attempting transition to:"
                         << m["Type"].toInt();
                break;
            }
        }
    }

    void Client::handleAuthenticatingAccountState(const QJsonObject& m)
    {
        switch(m["Type"].toInt())
        {
            case ProtocolManager::AccountAuthenticated:
            {
                qDebug() << "Account"
                         << current_user.get_username()
                         << "Authenticated";
                state = ClientState::Connected;
                emit accountAuthenticated();
                break;
            }
            case ProtocolManager::AccountNotAuthenticated:
            {
                qDebug() << "Authentication Failed, wrong code";
                emit accountAuthenticationFail();
                break;
            }
            default:
            {
                qDebug() << "State Transition Failure:"
                         << "\nCurrent State:" << state
                         << "\n  Attempting transition to:"
                         << m["Type"].toInt();
                break;
            }
        }
    }
    
};
