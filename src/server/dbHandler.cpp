#include <string>
#include <iostream>
#include "dbHandler.h"

bool dbHandler::availUsername(const Person & p)
{
    MYSQL_RES * result;
    std::string q = "select * from User where username='";
    q += p.u_.toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        printf(mysql_error(connection));
        return 0;
    }
    result = mysql_store_result(connection);
    bool avail = mysql_fetch_row(result) == NULL;
    mysql_free_result(result);
    return avail;
}

bool dbHandler::newUser(const Person & p)
{
    MYSQL_RES * result;
    std::string q = "insert User(username, password, email, permissions) values('" + p.u_.toStdString() + "', '" + p.pass_.toStdString() + "', '"
        + p.email_.toStdString() + "', ";
    q += (p.per_? "1" : "0");
    q += ")";
    if(mysql_query(connection, q.c_str()))
        return 0;
    q = "select * from User where username='" + p.u_.toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
        return 0;
    result = mysql_store_result(connection);
    bool success = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    return success;
}
