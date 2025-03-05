#ifndef USER_H
#define USER_H

#include <QtCore>

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
    
private:
    QString u_;
    QString pass_;
    QString email_;
    bool per_;
};

#endif
