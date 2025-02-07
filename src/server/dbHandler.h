#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtCore>
#include <stdio.h>
#include <mysql.h>

#include "User.h"

class dbHandler
{
public:
    dbHandler()
    {
        mysql_init(&mysql);
        connection = mysql_real_connect(&mysql, "127.0.0.1",
                                        "root", //CHANGE TO YOUR USER
                                        "root", //CHANGE TO YOUR PASSWORD
                                        "DSDB", 0, 0, 0);
        if(connection == NULL)
            printf(mysql_error(&mysql));
    }
    ~dbHandler()
    {
        mysql_close(connection);
    }

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
private:
    MYSQL * connection, mysql;
};  

#endif
