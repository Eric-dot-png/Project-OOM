// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <vector>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class ProtocolError{}; // occurs when too many arguments, or an
                       // undefined MessageType is passed to serialize()

class ProtocolManager // CLASS IS STATIC
{
private:
    ProtocolManager() {};
    ProtocolManager & operator=(const ProtocolManager & p) = delete;
    ProtocolManager(const ProtocolManager& p) = delete;
    
public:
    enum MessageType {
        START = '0', 
        LoginRequest, 
        LoginAccept, 
        LoginDenied, 
        CreateAccountRequest, 
        CreateAccountAccept, 
        CreateAccountDenied,  
        CreateAccountAuthCodeSubmit, 
        AccountAuthenticated,
        AccountNotAuthenticated,
        AnnounceIpPort,
        AnnounceOffline,
        PrivateMessageRequest,
        PrivateMessageAccept,
        PrivateMessageDenied,
        PrivateMessageForward,
        PrivateMessage,
        SIZE
    };
    
    // converts a message type list of QStrings into a QJsonObject,
    // then converts the object to QByteArray to send through sockets
    // see cpp for Json formats
    static QByteArray serialize(MessageType t, const QStringList& argv);
    
    // converts QByteArray to json object
    static QJsonObject deserialize(const QByteArray& data);
};

#endif
