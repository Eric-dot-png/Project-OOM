#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <unordered_set>

#include <QObject>
#include <QString>
#include <QList>

#include "message.h"

class ChatObject : public QObject
{
public:
    ChatObject(const QString& name,
               const std::unordered_set<QString>& members,
               const QString& owner,
               QObject * parent = NULL)
        : QObject(parent), name_(name), owner_(owner), members_(members),
          messages_({})
    {}

    virtual void prepend(const Message& message)
    {
        messages_.push_front(message);
    }
    
    virtual void sendMessage(const Message& message)
    {
        messages_.push_back(message);
    }
    
    virtual void addMember(const QString& username)
    {
        members_.insert(username);
    }
    
    virtual void removeMember(const QString& username)
    {
        auto p = members_.find(username);
        if (p != members_.end())
            members_.erase(p);
    }

    virtual std::unordered_set<QString> getMembers() const
    {
        return members_;
    }
    
    virtual void rename(const QString& new_name) { name_ = new_name; }

    virtual const QString& owner() const { return owner_; }
    
    const QString& name() const { return name_; }
    
    const QList<Message>& messages(quint32 start, quint32 end) const
    {
        return messages_.mid(start,end);
    }
    
    const QList<Message>& allMessages() const { return messages_; }

    quint32 size() const { return messages_.size(); }

    void clear() { messages_.clear(); }
    
protected:
    QString name_, owner_;
    std::unordered_set<QString> members_;
    QList<Message> messages_;
};

#endif 
