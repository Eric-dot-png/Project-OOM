// file : ProtocolManager.h
// name : eric

#include "ProtocolManager.h"

std::unordered_map<Protocol, Serializer*> ProtocolManager::map = {
    {Protocol::LoginRequest, LoginRequest::getInstance()},
    {Protocol::LoginDenied, LoginDenied::getInstance()},
    {Protocol::LoginAccept, LoginAccept::getInstance()},
    {Protocol::CreateAccountRequest, CreateAccountRequest::getInstance()},
    {Protocol::CreateAccountAccept, CreateAccountAccept::getInstance()},
    {Protocol::CreateAccountDenied, CreateAccountDenied::getInstance()},
    {Protocol::AuthCodeSubmit, AuthCodeSubmit::getInstance()},
    {Protocol::AuthCodeAccept, AuthCodeAccept::getInstance()},
    {Protocol::AuthCodeDenied, AuthCodeDenied::getInstance()},
    {Protocol::AnnounceOffline, AnnounceOffline::getInstance()},
    {Protocol::PrivateMessage, PrivateMessage::getInstance()},
    {Protocol::DiscoveryRequest, DiscoveryRequest::getInstance()},
    {Protocol::DiscoveryFail, DiscoveryFail::getInstance()},
    {Protocol::DiscoveryAccept, DiscoveryAccept::getInstance()}
};

QByteArray ProtocolManager::serialize(Protocol t, const QList<QJsonValue>& v)
{
    qDebug() << "Trying to serialize" << static_cast<int>(t) << v << '\n';
    
    return (*map.at(t))(v);
}

QJsonObject ProtocolManager::deserialize(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}
