#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtCore>
#include <stdio.h>
#include <mysql.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <QDateTime>
#include <list>

#include "User.h"

class dbHandler
{
public:
    static dbHandler * GetInstance();
    static void destroyInstance();
    
    //Checks if username is already used
    bool availUsername(const User &);
    //Adds new user to db, returns 0 if failed
    QString newUser(const User &, bool autoval = 0);
    //Validates user, returns 0 if something went wrong
    bool emailValidate(const User &, const QString & code);
    //Returns 1 if login credentials match a user
    bool loginValidate(const User &);
    //Returns 1 if registration credentials are removed
    bool removeReg(const User &);
    //Flushes out all expired registrations
    void cleanReg();
    //returns 1 if stored messages updated
    bool storeMessage(const QJsonObject &);
    //returns list of (length) messages beginning at start(0 is most recent message)
    std::list<QJsonObject> getMessages(const User &, const User &, int start, int length);
    
private:
    dbHandler();
    ~dbHandler();
    
    MYSQL * connection, mysql;
    static dbHandler * dbHandler_;
};  

#endif
