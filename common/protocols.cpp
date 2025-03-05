// file : protocols.cpp
// name : eric garcia

#include "protocols.h"

namespace Serializers
{
    QByteArray AbstractSerializer:: serializeUtil(Protocol t,
                                         const QList<QJsonValue>& argv,
                                          const QStringList& argn)
    {
        if (argv.size() != argn.size()) throw(ProtocolError());
        else
        {
            QJsonObject ret;
            int x = static_cast<int>(t);
            ret.insert("Type", x);
            for (int i=0;i<argv.size();++i)
                ret.insert(argn[i], argv[i]);
            return QJsonDocument(ret).toJson();
        }
    }

    LoginRequest * LoginRequest::instance(NULL);
    LoginAccept * LoginAccept::instance(NULL);
    LoginDenied * LoginDenied::instance(NULL);
    CreateAccountRequest * CreateAccountRequest::instance(NULL);
    CreateAccountAccept * CreateAccountAccept::instance(NULL);
    CreateAccountDenied * CreateAccountDenied::instance(NULL);
    AuthCodeSubmit * AuthCodeSubmit::instance(NULL);
    AuthCodeAccept * AuthCodeAccept::instance(NULL);
    AuthCodeDenied * AuthCodeDenied::instance(NULL);
    AnnounceOffline * AnnounceOffline::instance(NULL);
    PrivateMessage * PrivateMessage::instance(NULL);
    DiscoveryRequest * DiscoveryRequest::instance(NULL);
    DiscoveryAccept * DiscoveryAccept::instance(NULL);
    DiscoveryFail * DiscoveryFail::instance(NULL);
    FriendRequest * FriendRequest::instance(NULL);
    FriendAccept * FriendAccept::instance(NULL);
    FriendDenied * FriendDenied::instance(NULL);
    FriendRemoved * FriendRemoved::instance(NULL);
    
    void destroyInstances()
    {
        LoginRequest::destroyInstance();
        LoginAccept::destroyInstance();
        LoginDenied::destroyInstance();
        CreateAccountRequest::destroyInstance();
        CreateAccountAccept::destroyInstance();
        CreateAccountDenied::destroyInstance();
        AuthCodeSubmit::destroyInstance();
        AuthCodeAccept::destroyInstance();
        AuthCodeDenied::destroyInstance();
        AnnounceOffline::destroyInstance();
        PrivateMessage::destroyInstance();
        DiscoveryRequest::destroyInstance();
        DiscoveryFail::destroyInstance();
        DiscoveryAccept::destroyInstance();
        FriendRequest::destroyInstance();
        FriendAccept::destroyInstance();
        FriendDenied::destroyInstance();
        FriendRemoved::destroyInstance();
    }

};
