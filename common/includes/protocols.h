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
    ExtendMessageHistory,
    ExtendMessageHistoryAccept,
    ExtendMessageHistoryDenied,
    CreateGroupRequest,
    CreateGroupAccept,
    CreateGroupFail,
    GroupMessage,
    BlockUser,
    UnblockUser,
    GetGroupHistory,
    GetGroupHistorySuccess,
    LeaveGroup,
    AddGroupMember,
    TransferGroup
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
                    "FriendsList", "FriendRequestList", "Groups",
                    "Blocklist"});
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
                                 argv, {"Owner", "Name", "Members"});
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
                                 argv, {"Owner", "Name", "Members"});
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
                                 argv, {"Owner", "Name", "Message"});
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
    
    class GetGroupHistory : public AbstractSerializer,
                            public Singleton<GetGroupHistory>
    {
        friend class Singleton<GetGroupHistory>;
    private:
        GetGroupHistory() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::GetGroupHistory,
                                 argv, {"Owner","Name","CurrSize"});
        }
    };

    class GetGroupHistorySuccess : public AbstractSerializer,
                                   public Singleton<GetGroupHistorySuccess>
    {
        friend class Singleton<GetGroupHistorySuccess>;
    private:
        GetGroupHistorySuccess() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::GetGroupHistorySuccess,
                                 argv, {"Owner","Name","Messages"});
        }
    };

    class BlockUser : public AbstractSerializer, public Singleton<BlockUser>
    {
        friend class Singleton<BlockUser>;
    private:
        BlockUser() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::BlockUser, argv, {"From","To"});
        }
    };

    class UnblockUser : public AbstractSerializer,
                        public Singleton<UnblockUser>
    {
        friend class Singleton<UnblockUser>;
    private:
        UnblockUser() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::UnblockUser, argv, {"From","To"});
        }
    };

    class LeaveGroup : public AbstractSerializer,
                        public Singleton<LeaveGroup>
    {
        friend class Singleton<LeaveGroup>;
    private:
        LeaveGroup() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::LeaveGroup, argv, {"Owner","Name",
                    "User"});
        }
    };

    class AddGroupMember : public AbstractSerializer,
                           public Singleton<AddGroupMember>
    {
        friend class Singleton<AddGroupMember>;
    private:
        AddGroupMember() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::AddGroupMember, argv, {"Owner",
                    "Name", "User"});
        }
    };

    class TransferGroup : public AbstractSerializer,
                          public Singleton<TransferGroup>
    {
        friend class Singleton<TransferGroup>;
    private:
        TransferGroup() = default;
    public:
        QByteArray operator()(const QList<QJsonValue>& argv)
        {
            return serializeUtil(Protocol::TransferGroup, argv, {"Owner",
                    "Name", "User"});
        }
    };

    // destroy all serializer instances. called in the bottom of both main.cpp
    void destroyInstances();
};

#endif 
