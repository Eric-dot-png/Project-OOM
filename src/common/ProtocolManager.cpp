// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

namespace oom
{
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
            case AccountNotAuthenticated:
            case AccountAuthenticated:
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
            case CreateAccountAuthCodeSubmit:
            {
                if (argc == 3)
                {
                    QJsonObject ret({
                            {"Type", t},
                            {"Username", argv[0]},
                            {"Password", argv[1]},
                            {"Code", argv[2]}
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
