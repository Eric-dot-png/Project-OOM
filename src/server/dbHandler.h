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
    
    bool availUsername(const User &);
    bool newUser(const User &);
    bool loginValidate(const User &);
private:
    MYSQL * connection, mysql;
};  

#endif
