// file : protocols.h
// name : eric garcia

#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "singleton.h"

class ProtocolError{}; // this is an error that is thrown when:
                       //   a) the incorrect number of arguments is passed
                       //      to a serializer.
                       //   b) an invalid protocol index is called.

// types of protocols sent accross the network
enum class Protocol
{
    LoginRequest=0, 
    LoginAccept, 
    LoginDenied,
    CreateAccountRequest, 
    CreateAccountAccept, 
    CreateAccountDenied,  
    AuthCodeSubmit, 
    AuthCodeAccept,
    AuthCodeDenied,
    AnnounceOffline,
    PrivateMessage,
    DiscoveryRequest,
    DiscoveryFail, 
    DiscoveryAccept,
    FriendRequest, 
    FriendAccept,
    FriendDenied,
    FriendRemoved,
    FriendList,
    FriendListAccept,
    FriendListFailed,
    FriendRequestList,
    FriendRequestListAccept,
    FriendRequestListFailed,
    ExtendMessageHistory,
    ExtendMessageHistoryAccept,
    ExtendMessageHistoryDenied,
    CreateGroupRequest,
    CreateGroupAccept,
    CreateGroupFail,
    GroupMessage
};

/*
  What does a serializer do? Takes Arguments, and converts them to a
  formmatted byte array (using json).
  
  All serializers are singletons (other than the abstract base class)
  because you only need to have 1 serializer of each protocol (don't forget
  to initialize the static member in the protocols.cpp file).
  Every protocol includes a "Type" Field, where "Type" refers
  to the protocols type. Additionally, they contain additional fields
  that are more specific to the protocol's needs.
  
  For example, LoginRequest takes the additional fields "Username" and
  "Password", creating a json object with the fields "Type", "Username" and
  "Password". See the class (in this file) for its specific field names.
*/

// why using a namespace? to distinguish between Protocol and its respective
// serializer
namespace Serializers 
{
    class AbstractSerializer
    {
    public:
        virtual QByteArray operator()(const QList<QJsonValue>& argv) = 0;
    protected:
        AbstractSerializer() {}
        virtual ~AbstractSerializer() {};

        // This function is called by every child serializers operator()
        // function. The Protocol calls it using it's Protocol ("Type"),
        // and argument names as constants, and the arguments are the
        // parameters in its operator() function. 
        QByteArray serializeUtil(Protocol t, const QList<QJsonValue>& argv,
                                 const QStringList& argn);
    };
    
    class LoginRequest : public AbstractSerializer,
                         public Singleton<LoginRequest>
    {
        friend class Singleton<LoginRequest>;
    private:
        LoginRequest() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::LoginRequest,
                                 argv, {"Username", "Password"});
        }
    };

    class LoginAccept : public AbstractSerializer,
                        public Singleton<LoginAccept>
    {
        friend class Singleton<LoginAccept>;
    private:
        LoginAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::LoginAccept, argv, {"Username",
                    "FriendsList", "FriendRequestList", "Groups"});
        }
    };

    class LoginDenied : public AbstractSerializer,
                        public Singleton<LoginDenied>
    {
        friend class Singleton<LoginDenied>;
    private:
        LoginDenied() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::LoginDenied, argv,
                                 {"Error Message"});
        }
    };

    class CreateAccountRequest : public AbstractSerializer,
                                 public Singleton<CreateAccountRequest>
    {
        friend class Singleton<CreateAccountRequest>;
    private:
        CreateAccountRequest() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::CreateAccountRequest,
                                 argv, {"Username", "Password", "Email"});
        }
    };

    class CreateAccountAccept : public AbstractSerializer,
                                public Singleton<CreateAccountAccept>
    {
        friend class Singleton<CreateAccountAccept>;
    private:
        CreateAccountAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::CreateAccountAccept,
                                 argv, {"Username", "Password"});
        }
    };

    class CreateAccountDenied : public AbstractSerializer,
                                public Singleton<CreateAccountDenied>
    {
        friend class Singleton<CreateAccountDenied>;
    private:
        CreateAccountDenied() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::CreateAccountDenied,
                                 argv, {"Error Message"});
        }
    };

    class AuthCodeSubmit : public AbstractSerializer,
                           public Singleton<AuthCodeSubmit>
    {
        friend class Singleton<AuthCodeSubmit>;
    private:
        AuthCodeSubmit() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::AuthCodeSubmit,
                                 argv, {"Username","Password","Code"});
        }
    };

    class AuthCodeAccept : public AbstractSerializer,
                           public Singleton<AuthCodeAccept>
    {
        friend class Singleton<AuthCodeAccept>;
    private:
        AuthCodeAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::AuthCodeAccept, argv, {});
        }
    };

    class AuthCodeDenied : public AbstractSerializer,
                           public Singleton<AuthCodeDenied>
    {
        friend class Singleton<AuthCodeDenied>;
    private:
        AuthCodeDenied() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::AuthCodeDenied, argv,
                                 {"Error Message"});
        }
    };

    class AnnounceOffline : public AbstractSerializer,
                            public Singleton<AnnounceOffline>
    {
        friend class Singleton<AnnounceOffline>;
    private:
        AnnounceOffline() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::AnnounceOffline, argv,
                                 {"Username"});
        }
    };

    class PrivateMessage : public AbstractSerializer,
                           public Singleton<PrivateMessage>
    {
        friend class Singleton<PrivateMessage>;
    private:
        PrivateMessage() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::PrivateMessage,
                                 argv, {"To","From","Message"});
        }
    };

    class DiscoveryRequest : public AbstractSerializer,
                             public Singleton<DiscoveryRequest>
    {
        friend class Singleton<DiscoveryRequest>;
    private:
        DiscoveryRequest() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::DiscoveryRequest,
                                 argv, {"CurrUser", "Username"});
        }
    };
    
    class DiscoveryFail : public AbstractSerializer,
                          public Singleton<DiscoveryFail>
    {
        friend class Singleton<DiscoveryFail>;
    private:
        DiscoveryFail() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::DiscoveryFail, argv, {"Username"});
        }
    };

    class DiscoveryAccept : public AbstractSerializer,
                            public Singleton<DiscoveryAccept>
    {
        friend class Singleton<DiscoveryAccept>;
    private:
        DiscoveryAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::DiscoveryAccept, argv,
                                 {"Username", "Messages"});
        }
    };
    
    class FriendRequest : public AbstractSerializer,
                          public Singleton<FriendRequest>
    {
        friend class Singleton<FriendRequest>;
    private:
        FriendRequest() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendRequest,argv,{"From", "To"});
        }
    };

    class FriendAccept : public AbstractSerializer,
                         public Singleton<FriendAccept>
    {
        friend class Singleton<FriendAccept>;
    private:
        FriendAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendAccept,argv,{"From", "To"});
        }
    };

    
    class FriendDenied : public AbstractSerializer,
                         public Singleton<FriendDenied>
    {
        friend class Singleton<FriendDenied>;
    private:
        FriendDenied() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendDenied,argv,{"From", "To"});
        }
    };
    
    class FriendRemoved : public AbstractSerializer,
                          public Singleton<FriendRemoved>
    {
        friend class Singleton<FriendRemoved>;
    private:
        FriendRemoved() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendRemoved,argv,{"From", "To"});
        }
    };

    class FriendList : public AbstractSerializer,
                          public Singleton<FriendList>
    {
        friend class Singleton<FriendList>;
    private:
        FriendList() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendList,argv,{"From", "List"});
        }
    };


    class FriendListAccept : public AbstractSerializer,
                              public Singleton<FriendListAccept>
    {
        friend class Singleton<FriendListAccept>;
    private:
        FriendListAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendListAccept,argv,{"From", "List"});
        }
    };

    class FriendListFailed : public AbstractSerializer,
                             public Singleton<FriendListFailed>
    {
        friend class Singleton<FriendListFailed>;
    private:
        FriendListFailed() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendListFailed,argv,{"From", "List"});
        }
    };


    class FriendRequestList : public AbstractSerializer,
                          public Singleton<FriendRequestList>
    {
        friend class Singleton<FriendRequestList>;
    private:
        FriendRequestList() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendRequestList,argv,{"From", "List"});
        }
    };


    class FriendRequestListAccept : public AbstractSerializer,
                              public Singleton<FriendRequestListAccept>
    {
        friend class Singleton<FriendRequestListAccept>;
    private:
        FriendRequestListAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendRequestListAccept,argv,{"From", "List"});
        }
    };

    class FriendRequestListFailed : public AbstractSerializer,
                                    public Singleton<FriendRequestListFailed>
    {
        friend class Singleton<FriendRequestListFailed>;
    private:
        FriendRequestListFailed() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::FriendRequestListFailed,argv,
                                 {"From", "List"});
        }
    };

    class ExtendMessageHistory : public AbstractSerializer,
                                 public Singleton<ExtendMessageHistory>
    {
        friend class Singleton<ExtendMessageHistory>;
    private:
        ExtendMessageHistory() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::ExtendMessageHistory, argv,
                                 {"CurrUser", "Username", "Start"});
        }  
    };

    class ExtendMessageHistoryAccept : public AbstractSerializer,
                                 public Singleton<ExtendMessageHistoryAccept>
    {
        friend class Singleton<ExtendMessageHistoryAccept>;
    private:
        ExtendMessageHistoryAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::ExtendMessageHistoryAccept,
                                 argv, {"Username", "Messages"});
        }
    };
    
    class ExtendMessageHistoryDenied : public AbstractSerializer,
                                 public Singleton<ExtendMessageHistoryDenied>
    {
        friend class Singleton<ExtendMessageHistoryDenied>;
    private:
        ExtendMessageHistoryDenied() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::ExtendMessageHistoryDenied,
                                 argv, {"Username"});
        }
    };

    class CreateGroupRequest : public AbstractSerializer,
                               public Singleton<CreateGroupRequest>
    {
        friend class Singleton<CreateGroupRequest>;
    private:
        CreateGroupRequest() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::CreateGroupRequest,
                                 argv, {"Username", "GroupName", "Members"});
        }
    };

    class CreateGroupAccept : public AbstractSerializer,
                               public Singleton<CreateGroupAccept>
    {
        friend class Singleton<CreateGroupAccept>;
    private:
        CreateGroupAccept() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::CreateGroupAccept,
                                 argv, {"Username", "GroupName", "Members"});
        }
    };

    class CreateGroupFail : public AbstractSerializer,
                               public Singleton<CreateGroupFail>
    {
        friend class Singleton<CreateGroupFail>;
    private:
        CreateGroupFail() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::CreateGroupFail,
                                 argv, {"Username", "GroupName", "Message"});
        }
    };

    class GroupMessage : public AbstractSerializer,
                           public Singleton<GroupMessage>
    {
        friend class Singleton<GroupMessage>;
    private:
        GroupMessage() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::GroupMessage,
                                 argv, {"Owner","Name","From","Message"});
        }
    };

    // destroy all serializer instances. called in the bottom of both main.cpp
    void destroyInstances();
};

#endif 
