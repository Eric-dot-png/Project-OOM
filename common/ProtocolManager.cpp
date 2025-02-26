// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

QByteArray ProtocolManager::serialize(MessageType t,
                                      const QStringList& argv)
{
    // NOTE : the switch case below has certain cases
    //        overlap because they behave the same. I.e.
    //        the protocols require the same arguments
    
    int argc = argv.size();
    switch(t) // what type of message you want to make?
    {
        case AnnounceIpPort:
        {
            if (argc == 3)
            {
                QJsonObject ret({
                        {"Type", t},
                        {"Username", argv[0]},
                        {"Ip", argv[1]},
                        {"Port", argv[2]}
                    });
                return QJsonDocument(ret).toJson();
            }
            else throw(ProtocolError());
        }
        case AnnounceOffline:
        {
            if (argc == 1)
            {
                QJsonObject ret({
                        {"Type", t},
                        {"Username", argv[0]}
                    });
                return QJsonDocument(ret).toJson();
            }
            else throw(ProtocolError());
        }
        case PrivateMessageRequest:
        {
            if (argc == 2)
            {
                QJsonObject ret({
                        {"Type", t},
                        {"To", argv[0]},
                        {"Message", argv[1]}
                    });
                return QJsonDocument(ret).toJson();
            }
            else throw(ProtocolError());
        }
        case PrivateMessageAccept:
        {
            if (argc == 4)
            {
                QJsonObject ret({
                        {"Type", t},
                        {"To", argv[0]},
                        {"Ip", argv[1]},
                        {"Port", argv[2]},
                        {"Message", argv[3]}
                    });
                return QJsonDocument(ret).toJson();
            }
            else throw(ProtocolError());
        }
        case PrivateMessageDenied:
        {
            if (argc == 1)
            {
                QJsonObject ret({
                        {"Type", t},
                        {"Reason", argv[0]}
                    });
                return QJsonDocument(ret).toJson();
            }
            else throw(ProtocolError());
        }
        case PrivateMessageForward:
        case PrivateMessage:
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
            // in the case of CreateAccountAccept:
            //    meant to include username and password because
            //    when sent from server to client, client
            //    needs access to the account
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
        case AccountNotAuthenticated:
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
        case AccountAuthenticated:
        {
            if (argc == 0)
            {
                QJsonObject ret({
                        {"Type", t},
                    });
                return QJsonDocument(ret).toJson();
            }
            else throw(ProtocolError());
        }
        case DiscoveryFail:
        case DiscoveryAccept:
        case DiscoveryRequest:
        case LoginAccept:
        {
            if (argc == 1) // no argument is needed
            {
                QJsonObject ret({
                        {"Type", t},
                        {"Username", argv[0]}
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
    // QByteArray -> JsonDocument -> JsonObject
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}
