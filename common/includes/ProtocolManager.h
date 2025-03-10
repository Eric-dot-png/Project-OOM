// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <unordered_map>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "protocols.h"

// This class is static. No instances of it should be made. It's purpose
// is to handle the serialization and deserialization of messages
// accross the network. See protocols.cpp for specific message types.
class ProtocolManager
{
public:
    ProtocolManager & operator=(const ProtocolManager & p) = delete;
    ProtocolManager(const ProtocolManager& p) = delete;

    // Serializes arguments into byte array based on the protocol.
    //    uses map to map protocol type to protocol serializer, then
    //    uses that serializer to serialize message.
    static QByteArray serialize(Protocol t,const QList<QJsonValue>& argv);
    
    // converts QByteArray to back to json object
    static QJsonObject deserialize(const QByteArray& data);
private:
    ProtocolManager() {};
    ~ProtocolManager() {};
    
    static std::unordered_map<Protocol, Serializers::AbstractSerializer*> map;
};

#endif
