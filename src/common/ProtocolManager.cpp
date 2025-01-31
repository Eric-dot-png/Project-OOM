// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

namespace oom
{
    // ProtocolManager::MessageType ProtocolManager::classify(
    //     const QString& msg)
    // {
    //     char t = msg[0].unicode();
    //     if (t > START && t < SIZE) return static_cast<MessageType>(t); 
    //     else throw(ProtocolError());
    // }
    
    // QString ProtocolManager::contents(const QString& msg)
    // {
    //     QString cleaned = msg;
    //     cleaned.remove(0,1);
    //     return cleaned;
    // }
    
    // QByteArray ProtocolManager::constructMsg(MessageType t,
    //                                       const std::vector<QString>& argv)
    // {
    //     int argc = argv.size();
    //     QString p(t);
        
    //     switch(t)
    //     {
    //         case CreateAccountRequest:
    //         case MessageRequest:
    //         {
    //             if (argc == 3)
    //             {
    //                 QString delim = "";
    //                 for (const QString& arg : argv)
    //                 {
    //                     p += delim + arg;
    //                     delim = " ";
    //                 }
    //                 return p.toUtf8();
    //             }
    //             else throw(ProtocolError());
    //             break;
    //         }
    //         case CreateAccountAccept: // same for now
    //         case LoginRequest:
    //         {
    //             if (argc == 2) return (p + argv[0] + " " + argv[1]).toUtf8();
    //             else throw(ProtocolError());
    //         }
    //         case CreateAccountDenied: // same for now
    //         case LoginDenied: // same for now
    //         case LoginAccept:
    //         {
    //             if (argc == 0) return p.toUtf8();
    //             else throw(ProtocolError());
    //         }
            
    //         default: throw(ProtocolError());
    //     }
    // }

    QByteArray ProtocolManager::serialize(MessageType t,
                                          const QStringList& argv)
    {
        int argc = argv.size();
        switch(t)
        {
            case MessageRequest:
            {
                if (argc == 3)
                {
                    QJsonObject ret({
                        {"Type", t},
                        {"To", argv[0]},
                        {"From", argv[1]},
                        {"Message", argv[2]}
                        });
                    return QJsonDocument(ret).toJson();
                }
                else throw(ProtocolError());
            }
            case CreateAccountRequest:
            {
                if (argc == 3)
                {
                    QJsonObject ret({
                        {"Type", t},
                        {"Username", argv[0]},
                        {"Password", argv[1]},
                        {"Email", argv[2]}
                        });
                    return QJsonDocument(ret).toJson();
                }
                else throw(ProtocolError());
            }
            case CreateAccountAccept:
            case LoginRequest:
            {
                if (argc == 2)
                {
                    QJsonObject ret({
                        {"Type", t},
                        {"Username", argv[0]},
                        {"Password", argv[1]}
                        });
                    return QJsonDocument(ret).toJson();
                }
                
                else throw(ProtocolError());
            }
            case CreateAccountDenied: 
            case LoginDenied: 
            {
                if (argc == 1)
                {
                    QJsonObject ret({
                        {"Type", t},
                        {"Error Message", argv[0]}
                        });
                    return QJsonDocument(ret).toJson();
                }
                else throw(ProtocolError());
            }
            case LoginAccept:
            {
                if (argc == 0) 
                {
                    QJsonObject ret({
                        {"Type", t}
                        });
                    return QJsonDocument(ret).toJson();
                }
                else throw(ProtocolError());
            }
            default: throw(ProtocolError());
        }
    }

    QJsonObject ProtocolManager::deserialize(const QByteArray& data)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        return doc.object();
    }
};
