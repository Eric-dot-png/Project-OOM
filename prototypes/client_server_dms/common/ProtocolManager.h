// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <vector>
#include <QString>

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
            SIZE
        };
        
        static MessageType classify(const QString& msg);
        static QString contents(const QString& msg);
        
        static QByteArray constructMsg(MessageType t,
                                       const std::vector<QString>& argv);
    };
};

#endif
