#include "dbHandler.h"

bool dbHandler::availUsername(const Person & p)
{
    q.exec("SELECT username FROM User WHERE username='" + p.u_ + "'");
    return !q.next();
}

bool dbHandler::newUser(const Person & p)
{
    q.exec("INSERT INTO User (username,password,email,permissions) VALUES ('" + p.u_ + "', '" + p.pass_ + "', '" + p.email_ + "', "
           + QString::number(p.per_) + ")");
    q.exec("SELECT username FROM User where username='" + p.u_ + "'");
    return q.next();
}
