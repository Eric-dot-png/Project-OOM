// file : Client.cpp
// name : eric garcia

#include "Client.h"

namespace oom
{
    Client::Client(QObject * parent)
        : QObject(parent), state(ClientState::Disconnected),
          socket(new QTcpSocket(this))
    {
        connect(socket, &QTcpSocket::connected, this, [&](){
            qDebug() << "Connected to Server";
            state = ClientState::Connected;
        });
        
        connect(socket, &QTcpSocket::disconnected, this, [&](){
            qDebug() << "Disconnected from Server";
            state = ClientState::Disconnected;
        });

        connect(socket, &QTcpSocket::readyRead, this, &Client::onReply);
    }
    
    Client::~Client()
    {
        delete socket;
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
            socket->disconnectFromHost();
            state = Disconnecting;
        }
        else
        {
            qDebug() << "Already disconnected.";
        }
    }
    
    void Client::login(const QString& user, const QString& pwd)
    {
        if (state == ClientState::Connected)
        {
            qDebug() << "Attempting Login...";
            socket->write(ProtocolManager::constructMsg(
                              ProtocolManager::LoginRequest, {user, pwd})
                );
            state = ClientState::LoggingIn;
        }
        else
        {
            qDebug() << "Can't Login. Current State:" << state;
        }
    }

    void Client::createAccount(const QString& user, const QString& pwd)
    {
        if (state == ClientState::Connected)
        {
            qDebug() << "Attempting Create Account...";
            socket->write(ProtocolManager::constructMsg(
                              ProtocolManager::CreateAccountRequest,
                              {user, pwd})
                );
            state = ClientState::CreatingAccount;
        }
        else
        {
            qDebug() << "Can't Create Account. Current State:" << state;
        }
    }
    
    void Client::onReply()
    {
        QString data = socket->readAll();
        qDebug() << "Recieved" << data << "from server.";

        ProtocolManager::MessageType t = ProtocolManager::classify(data);
        QString msg = ProtocolManager::contents(data);
        
        switch(state)
        {
            case ClientState::LoggingIn:
            {
                switch(t)
                {
                    case ProtocolManager::LoginAccept:
                    {
                        qDebug() << "Login Success!";
                        state = ClientState::LoggedIn;
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
                                 << "\n  Attempting transition to:" << t;
                        state = ClientState::Connected;
                        break;
                    }
                }
                break;
            }
            case ClientState::CreatingAccount:
            {
                switch(t)
                {
                    case ProtocolManager::CreateAccountAccept:
                    {
                        QStringList ms = msg.split(' ');
                        state = ClientState::Connected;
                        qDebug() << "Account with username" << ms[0]
                                 << "created!";
                        login(ms[0],ms[1]); // potentially change?
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
                                 << "\n  Attempting transition to:" << t;
                        break;
                    }
                }
                break;
            }
            default:
            {
                qDebug() << "State Failure:" << "\nCurrentState: " << state;
                break;
            }
        }
    }
};
