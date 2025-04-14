// file : FriendList.h
// name : eric garcia

#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <unordered_set>
#include <QString>

inline
std::unordered_set<QString> convert(const QStringList& li)
{
    std::unordered_set<QString> ret;
    for (const QString& s : li)
        ret.insert(s);
    return ret;
}

/*
  Holds incoming and outgoing friend requests as well as current friends.
  Also handles friend logic.
 */
class FriendList
{
public:
    FriendList()
        : incoming_({}), outgoing_({}), friends_({})
    {}

    FriendList(const std::unordered_set<QString>& in,
               const std::unordered_set<QString>& out,
               const std::unordered_set<QString>& friends)
        : incoming_(in), outgoing_(out), friends_(friends)
    {}
    
    FriendList(const QStringList& in,
               const QStringList& out,
               const QStringList& friends)
        : FriendList(convert(in), convert(out), convert(friends))
    {}

    FriendList(const FriendList&) = default;
    FriendList & operator=(const FriendList&) = default;

    const std::unordered_set<QString>& incoming() const { return incoming_; }
    const std::unordered_set<QString>& outgoing() const { return outgoing_; }
    const std::unordered_set<QString>& friends() const { return friends_; }
    
    void sendRequest(const QString & user)
    {
        outgoing_.insert(user);
    };

    void recieveRequest(const QString& user)
    {
        incoming_.insert(user);
    }

    void denyRequest(const QString& user)
    {
        auto search = incoming_.find(user);
        if (search != incoming_.end()) incoming_.erase(search);
    }

    void acceptRequest(const QString& user)
    {
        auto search = incoming_.find(user);
        if (search != incoming_.end())
        {
            friends_.insert(user);
            incoming_.erase(search);
        }
    }

    void requestAccepted(const QString& user)
    {
        auto search = outgoing_.find(user);
        if (search != outgoing_.end())
        {
            friends_.insert(user);
            outgoing_.erase(search);
        }
    }

    void requestDenied(const QString& user)
    {
        auto search = outgoing_.find(user);
        if (search != outgoing_.end())
            outgoing_.erase(search);
    }

    void unfriend(const QString& user)
    {
        auto search = friends_.find(user);
        if (search != friends_.end())
            friends_.erase(search); 
    }
    
    bool isFriend(const QString& user) const
    {
        return friends_.find(user) != friends_.end();
    }

    bool isRequesting(const QString& user) const
    {
        return incoming_.find(user) != incoming_.end();
    }

    bool isRequested(const QString& user) const
    {
        return outgoing_.find(user) != outgoing_.end();
    }
    
private:
    std::unordered_set<QString> incoming_, outgoing_, friends_;
};



#endif
