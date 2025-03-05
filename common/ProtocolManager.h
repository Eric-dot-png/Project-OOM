// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <unordered_map>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "protocols.h"

class ProtocolManager
{
public:
    ProtocolManager & operator=(const ProtocolManager & p) = delete;
    ProtocolManager(const ProtocolManager& p) = delete;
    
    // converts a message type list of QStrings into a QJsonObject,
    // then converts the object to QByteArray to send through sockets
    // see cpp for Json formats
    static QByteArray serialize(Protocol t,const QList<QJsonValue>& argv);
    
    // converts QByteArray to json object
    static QJsonObject deserialize(const QByteArray& data);
private:
    ProtocolManager() {};
    ~ProtocolManager() {};
    
    static std::unordered_map<Protocol, Serializer*> map;
};

#endif
