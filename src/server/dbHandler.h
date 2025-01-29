#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtCore>
#include <QtSql>
#include <iostream>

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
    dbHandler() {}
    bool availUsername(const Person &);
    bool newUser(const Person &);
private:
    QSqlQuery q;
};  

#endif
