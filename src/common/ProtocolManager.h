// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <vector>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

namespace oom
{
    class ProtocolError{};
    
    class ProtocolManager 
    {
    private:
        ProtocolManager() {};
        
    public:
        enum MessageType {
            START = '0',
            LoginRequest,
            LoginAccept,
            LoginDenied,
            CreateAccountRequest,
            CreateAccountAccept,
            CreateAccountDenied,
            MessageRequest,
            MessageAccept,
            MessageDenied,
            MessageForward,
            SIZE
        };
        
        static QByteArray serialize(MessageType t, const QStringList& argv);
        static QJsonObject deserialize(const QByteArray& data);
    };
};

#endif
