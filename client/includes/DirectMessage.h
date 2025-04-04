#ifndef DIRECTMESSAGE_H
#define DIRECTMESSAGE_H

#include "ChatObject.h"

class DirectMessage : public ChatObject
{
public:
    DirectMessage(const QString& current,
                  const QString& other,
                  QObject * parent = NULL)
        : ChatObject(other, {current, other}, "", parent) // "" = no owner
    {}
    
    void addMember(const QString& username)
    {
        throw std::runtime_error("Cant add users to a Direct Message!");
    }

    void removeMember(const QString& username)
    {
        throw std::runtime_error("Cant remove users from a Direct Message!");
    }

    void rename(const QString& new_name)
    {
        throw std::runtime_error("Cant rename a Direct Message!");
    }

    const QString& owner() const
    {
        throw std::runtime_error("Direct Messages do not have an owner!");
    }
};

#endif 
