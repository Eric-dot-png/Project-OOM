// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <vector>
#include <QString>

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
        SIZE
    };

    static MessageType classify(const QString& msg);
    static QString contents(const QString& msg);
    
    static QString constructMsg(MessageType t,int argc,
                                const std::vector<QString> argv);
};

#endif
