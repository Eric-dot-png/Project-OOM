// file : ProtocolManager.cpp
// name : eric garcia

#include "ProtocolManager.h"

// protocol serializer instance map
std::unordered_map<Protocol,
                   Serializers::AbstractSerializer*> ProtocolManager::map = {
    {Protocol::LoginRequest, Serializers::LoginRequest::getInstance()},
    {Protocol::LoginDenied, Serializers::LoginDenied::getInstance()},
    {Protocol::LoginAccept, Serializers::LoginAccept::getInstance()},
    {Protocol::CreateAccountRequest,
     Serializers::CreateAccountRequest::getInstance()},
    {Protocol::CreateAccountAccept,
     Serializers::CreateAccountAccept::getInstance()},
    {Protocol::CreateAccountDenied,
     Serializers::CreateAccountDenied::getInstance()},
    {Protocol::AuthCodeSubmit, Serializers::AuthCodeSubmit::getInstance()},
    {Protocol::AuthCodeAccept, Serializers::AuthCodeAccept::getInstance()},
    {Protocol::AuthCodeDenied, Serializers::AuthCodeDenied::getInstance()},
    {Protocol::AnnounceOffline, Serializers::AnnounceOffline::getInstance()},
    {Protocol::PrivateMessage, Serializers::PrivateMessage::getInstance()},
    {Protocol::DiscoveryRequest,Serializers::DiscoveryRequest::getInstance()},
    {Protocol::DiscoveryFail, Serializers::DiscoveryFail::getInstance()},
    {Protocol::DiscoveryAccept, Serializers::DiscoveryAccept::getInstance()},
    {Protocol::FriendRequest, Serializers::FriendRequest::getInstance()},
    {Protocol::FriendAccept, Serializers::FriendAccept::getInstance()},
    {Protocol::FriendDenied, Serializers::FriendDenied::getInstance()},
    {Protocol::FriendRemoved, Serializers::FriendRemoved::getInstance()},
    {Protocol::FriendList, Serializers::FriendList::getInstance()},
    {Protocol::FriendListAccept, Serializers::FriendListAccept::getInstance()},
    {Protocol::FriendListFailed, Serializers::FriendListFailed::getInstance()},
    {Protocol::FriendRequestList, Serializers::FriendRequestList::getInstance()},
    {Protocol::FriendRequestListAccept, Serializers::FriendRequestListAccept::getInstance()},
    {Protocol::FriendRequestListFailed, Serializers::FriendRequestListFailed::getInstance()}
};

QByteArray ProtocolManager::serialize(Protocol t, const QList<QJsonValue>& v)
{
    return (*map.at(t))(v);
}

QJsonObject ProtocolManager::deserialize(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}
