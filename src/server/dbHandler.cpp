#include <string>
#include <iostream>
#include <cstdlib>
#include <QDateTime>
#include "dbHandler.h"

//Returns 1 if select statement where username=desired username is empty
bool dbHandler::availUsername(const User & p)
{
    MYSQL_RES * result;
    std::string q = "select * from User where username='";
    q += p.get_username().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    bool avail = mysql_fetch_row(result) == NULL;
    if(!avail) return 0;
    mysql_free_result(result);
    q = "select * from Registration where username='"
        + p.get_username().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    avail = mysql_fetch_row(result) == NULL;
    mysql_free_result(result);
    return avail;
}

//Inserts User, and returns 0 if anything goes wrong
bool dbHandler::newUser(const User & p)
{
    MYSQL_RES * result;
    std::string timer = QDateTime::currentDateTime().addSecs(1800).toString("yyyy-MM-dd hh:mm:ss").toStdString();
    std::string valcode = "";
    for(int i = 0; i < 6; i++)
        valcode += '0' + rand() % 10;
    std::string q = "insert Registration(username, password, email, permissions) values('" + p.get_username().toStdString() + "', '"
        + p.get_password().toStdString() + "', '"
        + p.get_email().toStdString() + "', ";
    q += (p.get_permissions()? "1" : "0");
    q += ")";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "first false" << mysql_error(connection);
        return 0;
    }
    q = "select * from Registration where username='"
        + p.get_username().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "second false";
        return 0;
    }
    result = mysql_store_result(connection);
    bool success = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    return success;
}

//Returns 1 if user, password pair exists in validated users
bool dbHandler::loginValidate(const User & p)
{
    MYSQL_RES * result;
    std::string q = "select * from User where username='"
        + p.get_username().toStdString() + "' and password='"
        + p.get_password().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
        return 0;
    result = mysql_store_result(connection);
    bool success = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    return success;
}
