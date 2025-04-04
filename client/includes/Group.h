#ifndef GROUP_H
#define GROUP_H

#include <unordered_set>

#include <QString>

#include "ChatObject.h"

inline
QString getGroupName(const std::unordered_set<QString>& members)
{
    QString ret, delim = "";
    for (const QString& m : members)
    {
        ret += delim + m;
        delim = ", ";
    }
    return ret;
}

class Group : public ChatObject
{
public:
    Group(const std::unordered_set<QString>& members, const QString& owner,
          QObject * parent = NULL)
        : ChatObject(getGroupName(members), members, owner, parent)
    {}
    
    Group(const std::unordered_set<QString> & members, const QString & owner,
          const QString & name, QObject * parent = NULL)
        : ChatObject(name, members, owner, parent)
    {}
};

#endif 
