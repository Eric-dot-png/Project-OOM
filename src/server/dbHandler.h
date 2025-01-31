#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtCore>
#include <stdio.h>
#include <mysql.h>

class Person
{
public:
    Person(const QString & u,
           const QString & pass,
           const QString & e,
           bool per=0)
        : u_(u), pass_(pass), email_(e), per_(per)
    {};

    QString u_;
    QString pass_;
    QString email_;
    bool per_;
};

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
    
    bool availUsername(const Person &);
    bool newUser(const Person &);

private:
    MYSQL * connection, mysql;
};  

#endif
