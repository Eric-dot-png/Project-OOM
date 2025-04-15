// file : Client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <unordered_map>

#include <QObject>
#include <QDebug>
#include <QTime>

#include "config.h"
#include "singleton.h"
#include "User.h"
#include "NetworkManager.h"
#include "ChatObject.h"
#include "DirectMessage.h"
#include "Group.h"
#include "FriendList.h"
#include "BlockList.h"

class Client : public QObject, public Singleton<Client>
{
    Q_OBJECT
    friend class Singleton<Client>;
public:
    Client(Client&) = delete;
    Client& operator=(const Client&) = delete;
    
    //AbstractState * getState() const { return state; }
    const User& getUser() const { return current_user; }
    User& getUser() { return current_user; }

    // must be in Disconnected state.
    // puts the client into Connecting state
    void connectToServer(const QHostAddress& host, int port);
    
    // must not be in Disconnected state
    // puts the client into Disconnected state
    void disconnect();
    
    // must be in Connected state
    // puts the client into loggingin state
    void login(const User &);

    // must be in Connected state
    // puts the client into CreatingAccount state
    void createAccount(const User &);

    // must be in AuthenticatingAccount state
    // puts the client into  Connected State
    void submitAuthCode(const QString&);

    // attempts to discover a user. the idea is to see if a user
    // exists to private message, if they do, their
    // message history is sent as well as their username
    void discover(const User& u);

    void privateMessage(const User& u, const QString& message);
    
    void friendRequest(const User& u);
    
    void acceptFriend(const User& u);

    void denyFriend(const User& u);
    
    void removeFriend(const User& u);

    void block(const User& u);

    void unblock(const User& u);

    const FriendList& getFriendList() const { return friendlist; }
    BlockList getBlockList();
    
    // this one right below not needed anymore...
    void extendMessageHistory(const User& u, quint32 currentSize);
    void extendMessageHistory(const User&u);
    
    const ChatObject * getDMsWith(const QString& u) const;
    
    void createGroup(const QString & name, const QStringList & members) const;
    void messageGroup(const QString & owner, const QString & name,
                      const QString & message);
    ChatObject * getGroupHistory(const QString & owner,
                                 const QString & name) const;
    void extendGroupHistory(const QString & owner,
                            const QString & name) const;
                                                       
public slots:
    // must be in loggedin state
    // puts the client into connected state
    void logout();
    
signals: 
    void connectedToServer();
    void disconnectedFromServer();
    void loginSuccess();
    void loginFail();
    void accountNotCreated();
    void accountCreated();
    void accountAuthenticated();
    void accountAuthenticationFail();
    void discoverUserFail(const QString& username);
    void discoverUserSucceed(const QString& username,
                             const QJsonArray & messageJsonList);
    void recievedDM(const QString& from, const QString& msg);
    void recievedMoreMsgs(const QList<Message> list);
    void recievedFriendRequest(const QString& from);
    void recievedFriendRemove(const QString& from);
    void friendAccepted(const QString& from);
    
    void extendMsgSucceed(const QString& user,const QJsonArray & msgs);
    void createGroupSucceed(const QString & name,
                            const QStringList & members);
    void createGroupDeny(const QString & err);
    void recievedGroupMessage(const QString & owner, const QString & name,
                              const QString & from, const QString & message);
                                                                            
private slots: // these are functions that are connected to signals
    void initializeSession(const QString& username,
                           const QStringList& friends,
                           const QStringList& friendRqs,
                           const QJsonArray & groups,
                           const QStringList & blocklist);
    void initializeDMs(const QString& user, const QJsonArray & msgs);
    void initializeGroups(const QString & owner, const QString & name,
                          const std::unordered_set<QString> & members,
                          const QJsonArray & messages);
    void handleDM(const QString& user, const QString& msg);
    void handleGroupMessage(const QString & owner, const QString & name,
                            const QString & from, const QString & message);
    void handleMoreMsgs(const QString& user, const QJsonArray & messages);
    void handleGroupHistoryFound(const QString & owner, const QString & name,
                                 const QJsonArray & messages);
private:
    Client();
    ~Client();

    enum class ClientState
    {
        Disconnected = 0,
        Disconnecting,
        Connecting,
        Connected,
        LoggingIn,
        LoggedIn,
        CreatingAccount
    };

    QString dmKey(const QString&) const;
    QString dmKey(const User&) const;
    QString groupKey(const QString &, const QString &) const;

    std::unordered_map<QString, ChatObject*> chats;
    
    NetworkManager * nw;
    ClientState state;
    User current_user;

    FriendList friendlist;
    BlockList blocklist;
};

#endif
