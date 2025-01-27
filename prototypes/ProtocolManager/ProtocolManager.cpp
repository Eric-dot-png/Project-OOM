// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

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

QString ProtocolManager::constructMsg(MessageType t, int argc,
                                      const std::vector<QString> argv)
{
    QString p(t);
    switch(t)
    {
        case LoginRequest:
            if (argc == 2)
                return p + QString("%2 %3").arg(argv[0]).arg(argv[1]);
            else
                throw(ProtocolError());
        case LoginAccept:
            if (argc == 0)
                return p;
            else
                throw(ProtocolError());
        default:
            throw(ProtocolError());
    }
}
