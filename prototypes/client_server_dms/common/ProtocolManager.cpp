// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

namespace oom
{
    ProtocolManager::Protocol ProtocolManager::classify(
        const QString& msg)
    {
        char t = msg[0].unicode();
        if (t > START && t < SIZE) return static_cast<Protocol>(t); 
        else throw(ProtocolError());
    }
    
    QString ProtocolManager::contents(const QString& msg)
    {
        QString cleaned = msg;
        cleaned.remove(0,1);
        return cleaned;
    }
    
    QByteArray ProtocolManager::constructMsg(Protocol t,
                                             const QStringList& argv)
    {
        int argc = argv.size();
        QString p(t);
        QByteArray ret = (p + argv.join(" ")).toUtf8();
        switch(t)
        {
            case CreateAccountRequest: // same for now
            case MessageRequest:
            {
                if (argc == 3) return ret;
                else throw(ProtocolError());
            }
            
            case CreateAccountAccept: // same for now
            case LoginRequest:
            {
                if (argc == 2) return ret;
                else throw(ProtocolError());
            }
            case CreateAccountDenied: // same for now
            case LoginDenied: // same for now
            case LoginAccept:
            {
                if (argc == 0) return ret;
                else throw(ProtocolError());
            }
            default: throw(ProtocolError());
        }
    }
    
    // QJsonObject ProtocolManager::deserialize(const QByteArray& msg)
    // {
    //     return QJsonDocument::fromJson(msg).object();
    // }
    
    // QByteArray ProtocolManager::serialize(Protocol t,const QStringList& argv);
    // {
    //     int argc = argv.size();
        
    //     switch(t)
    //     {
    //         case CreateAccountRequest:
    //         case CreateAccountAccept:
    //         case LoginRequest:
    //         {
    //             if (argc == 2)
    //             {
    //                 QJsonObject obj{
    //                     { "Protocol" , t },
    //                     { "Username" , argv[0]},
    //                     { "Password" , argv[1]}
    //                 };
    //                 return QJsonDocument(obj).toJson();
    //             }
    //             else throw(ProtocolError());
    //         }
    //         case LoginDenied:
    //         case CreateAccountDenied:
    //         {
    //             if (argc == 1)
    //             {
    //                 QJsonObject obj{
    //                     { "Protocol" , t },
    //                     { "Error Message" , argv[0]}
    //                 };
    //                 return QJsonDocument(obj).toJson();
    //             }
    //             else throw(ProtocolError());
    //         }
    //         case LoginAccept:
    //         {
    //             if (argc == 0)
    //             {
    //                 QJsonObject obj{
    //                     { "Protocol" , t }
    //                 };
    //                 return QJsonDocument(obj).toJson();
    //             }
    //             else throw(ProtocolError());
    //         }
    //         case MessageRequest:
    //         {
    //             if (argc == 3)
    //             {
    //                 QJsonObject obj{
    //                     { "Protocol" , t },
    //                     { "Sender" , argv[0]},
    //                     { "Reciever" , argv[1]},
    //                     { "Message" , argv[2]}
    //                 };
    //                 return QJsonDocument(obj).toJson();
    //             }
    //             else throw(ProtocolError());
    //         }
    //         default: throw(ProtocolError());
    //     }
    // }
};
