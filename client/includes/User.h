#ifndef USER_H
#define USER_H

#include <QtCore>
#include <QWidget>
#include <QMetaType>

class User
{

public:
    User(const QString & u,
         const QString & pass="",
         const QString & e="",
         bool per=0)
        : u_(u), pass_(pass), email_(e), per_(per)
    {}

    QString get_username() const
    {
        return u_;
    }

    QString get_password() const
    {
        return pass_;
    }

    QString get_email() const
    {
        return email_;
    }
    
    bool get_permissions() const
    {
        return per_;
    }

    void setFriendList(const QStringList& list)
    {
        friendList = list;
    }

    void setFriendRequestList(const QStringList& list)
    {
        friendRequestList = list;
    }
    
    const QStringList& getFriendRequestList()
    {
        return friendRequestList;
    }
    const QStringList& getFriendsList()
    {
        return friendList;
    }

    void removeFriend(const QString& user)
    {
        friendList.removeOne(user);
    }

    void removeFriendRequest(const QString& user)
    {
        friendRequestList.removeOne(user);
    }
    
    void addFriendRequestList(const QString& user)
    {
        friendRequestList.append(user);
    }
    
    void addFriend(const QString& user)
    {
        friendList.append(user);
    }
    
    //QVariant needs these idfk why. it wont compile without them
    //Also qHash below

    User() = default;
    User(const User &other) = default;
    User& operator=(const User& o) = default;
    bool operator==(const User &o) const
    {
        return u_ == o.u_;
    }

    
private:
    QString u_;
    QString pass_;
    QString email_;
    bool per_;

    QStringList friendList;
    QStringList friendRequestList;


};

inline uint qHash(const User &key, uint seed = 0)
{
    return qHash(key.get_username(), seed);
}

const User GUEST("Guest");

Q_DECLARE_METATYPE(User)
#endif
