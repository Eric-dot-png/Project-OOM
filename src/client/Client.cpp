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
    
    void Client::onReply()
    {
        QByteArray data = socket->readAll();
        qDebug() << "Recieved" << data << "from server.";

        QJsonObject m = ProtocolManager::deserialize(data);
        
        switch(state)
        {
            case ClientState::LoggingIn:
            {
                switch(m["Type"].toInt())
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
                                 << "\n  Attempting transition to:"
                                 << m["Type"].toInt();
                        state = ClientState::Connected;
                        break;
                    }
                }
                break;
            }
            case ClientState::CreatingAccount:
            {
                switch(m["Type"].toInt())
                {
                    case ProtocolManager::CreateAccountAccept:
                    {
                        state = ClientState::Connected;
                        User u(QString(m["Username"].toString()),
                               QString(m["Password"].toString()));
                        qDebug() << "Account with username"
                                 << u.get_username()
                                 << "created!";
                        login(u);
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
