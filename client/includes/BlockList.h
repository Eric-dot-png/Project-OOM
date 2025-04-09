// file : BlockList.h
// name : eric garcia

#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <unordered_set>
#include <QString>

#include "FriendList.h" // for convert()

/*
  Holds usernames that are blocked.
 */
class BlockList
{
public:
    BlockList()
        : blocked_({})
    {}

    BlockList(const std::unordered_set<QString>& li)
        : blocked_(li)
    {}

    BlockList(const QStringList& li)
        : blocked_(convert(li))
    {}

    BlockList(const BlockList&) = default;
    BlockList& operator=(const BlockList&) = default;
    
    const std::unordered_set<QString>& blocked() const { return blocked_; }

    void add(const QString& user) { blocked_.insert(user); }
    void remove(const QString& user)
    {
        auto search = blocked_.find(user);
        if (search != blocked_.end()) blocked_.erase(search);
    }

    bool isBlocked(const QString& user)
    {
        return blocked_.find(user) != blocked_.end();
    }
private:
    std::unordered_set<QString> blocked_;
};

#endif 
