#ifndef USER_H
#define USER_H

#include <QtCore>
#include <QMetaType>

class User
{
public:
    User(const QString & u,
         const QString & pass="",
         const QString & e="",
         bool per=0)
        : u_(u), pass_(pass), email_(e), per_(per)
    {};

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

    //QVariant needs these idfk why. it wont compile without them
    //Also qHash below
    User() = default;
    User(const User &other) = default;
    bool operator==(const User &o) const
    {
        return u_ == o.u_;
    }


    
private:
    QString u_;
    QString pass_;
    QString email_;
    bool per_;
};

inline uint qHash(const User &key, uint seed = 0)
{
    return qHash(key.get_username(), seed);
}

Q_DECLARE_METATYPE(User)
#endif
