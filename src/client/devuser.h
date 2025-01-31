//Name: Alex Franke
//File: devuser.h
//OOM project


#ifndef DEVUSER_H
#define DEVUSER_H

#include "baseuser.h"

class DevUser : public BaseUser
{
    Q_OBJECT
public:
    explicit DevUser(QObject *parent = nullptr);
    DevUser(int id, const QString &username, QObject *parent = nullptr);
    ~DevUser() override;

};

#endif // DEVUSER_H
