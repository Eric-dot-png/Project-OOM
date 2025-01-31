//Name: Alex Franke
//File: user.h
//OOM project


#ifndef USER_H
#define USER_H

#include "baseuser.h"

class User : public BaseUser
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    User(int id, const QString &username, QObject *parent = nullptr);
    ~User() override;
};

#endif // USER_H
