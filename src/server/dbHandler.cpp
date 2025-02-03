#include <string>
#include <iostream>
#include "dbHandler.h"

bool dbHandler::availUsername(const User & p)
{
    MYSQL_RES * result;
    std::string q = "select * from User where username='";
    q += p.get_username().toStdString() + "'";
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

bool dbHandler::newUser(const User & p)
{
    MYSQL_RES * result;
    std::string q = "insert User(username, password, email, permissions) values('" + p.get_username().toStdString() + "', '"
        + p.get_password().toStdString() + "', '"
        + p.get_email().toStdString() + "', ";
    q += (p.get_permissions()? "1" : "0");
    q += ")";
    if(mysql_query(connection, q.c_str()))
        return 0;
    q = "select * from User where username='" + p.get_username().toStdString()
        + "'";
    if(mysql_query(connection, q.c_str()))
        return 0;
    result = mysql_store_result(connection);
    bool success = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    return success;
}

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
