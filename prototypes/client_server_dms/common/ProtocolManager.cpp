// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

namespace oom
{
    ProtocolManager::MessageType ProtocolManager::classify(
        const QString& msg)
    {
        char t = msg[0].unicode();
        if (t > START && t < SIZE) return static_cast<MessageType>(t); 
        else throw(ProtocolError());
    }
    
    QString ProtocolManager::contents(const QString& msg)
    {
        QString cleaned = msg;
        cleaned.remove(0,1);
        return cleaned;
    }
    
    QByteArray ProtocolManager::constructMsg(MessageType t,
                                          const std::vector<QString>& argv)
    {
        int argc = argv.size();
        QString p(t);
        
        switch(t)
        {
            case LoginRequest:
                if (argc == 2)
                    return (p + argv[0] + " " + argv[1]).toUtf8();
                else
                    throw(ProtocolError());
            case LoginAccept:
                if (argc == 0)
                    return p.toUtf8();
                else
                    throw(ProtocolError());
            default:
                throw(ProtocolError());
        }
    }
};
