#include "dbHandler.h"

dbHandler * dbHandler::dbHandler_ = NULL;

dbHandler::dbHandler()
{
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1",
                                    "root", //CHANGE TO YOUR USER
                                    "root", //CHANGE TO YOUR PASSWORD
                                    "DSDB", 0, 0, 0);
    if(connection == NULL)
        printf(mysql_error(&mysql));
}

dbHandler::~dbHandler()
{
    mysql_close(connection);
}

dbHandler * dbHandler::GetInstance()
{
    if(dbHandler_ == NULL)
        dbHandler_ = new dbHandler();
    return dbHandler_;
}

void dbHandler::destroyInstance()
{
    if (dbHandler_ != NULL)
    {
        delete dbHandler_;
        dbHandler_ = NULL;
    }
}

//Returns 1 if select statement where username=desired username is empty
bool dbHandler::availUsername(const User & p)
{
    MYSQL_RES * result;
    
    // SELECT * FROM User WHERE username='(username)'
    std::string q = "select * from User where username='";
    q += p.get_username().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    bool avail = mysql_fetch_row(result) == NULL;
    mysql_free_result(result);
    if(!avail) // if select statement does not give emptyset
        return 0;

    // SELECT * FROM Registration WHERE username='(username)'
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
    return avail; // returns whether select statement gives emptyset
}

//Inserts User, and returns 0 if anything goes wrong
QString dbHandler::newUser(const User & p, bool autoval)
{
    MYSQL_RES * result;
    if(autoval) // If bypassing Registration(either testing or already validated)
    {
        
        // INSERT User(username, password, email, permissions)
        // VALUES('(username)', '(password)', '(email)', permissions)
        std::string q = "insert User(username, password, email, permissions) values('" + p.get_username().toStdString() + "', '"
            + p.get_password().toStdString() + "', '"
            + p.get_email().toStdString() + "', ";
        q += (p.get_permissions()? "1": "0");
        q += ")";
        if(mysql_query(connection, q.c_str()))
        {
            qDebug() << "autoval fail " << mysql_error(connection);
            return "";
        }

        // SELECT * FROM User WHERE username='(username)'
        q = "select * from User where username='"
            + p.get_username().toStdString() + "'";
        if(mysql_query(connection, q.c_str()))
        {
            qDebug() << "new User not in user.. " << mysql_error(connection);
            return "";
        }
        result = mysql_store_result(connection);
        mysql_free_result(result);
        return "validated";
    }
    else // if requires validation
    {

        // INSERT Registration(username, password, email, permissions, timer, code)
        // VALUES('(username)', '(password)', '(email)', (permissions), '(timer)', '(valcode)')
        std::string timer = QDateTime::currentDateTime().addSecs(1800).toString("yyyy-MM-dd hh:mm:ss").toStdString();
        std::string valcode = "";
        for(int i = 0; i < 6; i++)
            valcode += '0' + rand() % 10;
        std::string q = "insert Registration(username, password, email, permissions, validTimeout, code) values('" + p.get_username().toStdString() + "', '"
            + p.get_password().toStdString() + "', '"
            + p.get_email().toStdString() + "', ";
        q += (p.get_permissions()? "1" : "0");
        q += ", '" + timer + "', '" + valcode + "')";
        if(mysql_query(connection, q.c_str()))
        {
            qDebug() << "first false" << mysql_error(connection);
            return "";
        }

        // SELECT * FROM Registration WHERE username='(username)'
        q = "select * from Registration where username='"
            + p.get_username().toStdString() + "'";
        if(mysql_query(connection, q.c_str()))
        {
            qDebug() << "second false";
            return "";
        }
        result = mysql_store_result(connection);
        mysql_free_result(result);
        
        return valcode.c_str(); // returns validation code
    }
}

//returns 1 if user info moved from registration to user
bool dbHandler::emailValidate(const User & p, const QString & code)
{
    MYSQL_RES * result;
    MYSQL_ROW row;

    // SELECT * FROM Registraion WHERE username='(username)' and code='(code)'
    std::string q = "select * from Registration where username='"
        + p.get_username().toStdString() + "' and code='" + code.toStdString()
        + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "Uh oh:" << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    if(row == NULL) // If no user/code combo in table Registration
    {
        qDebug() << "Information not in Registration: "
                 << mysql_error(connection);
        return 0;
    }
    User u(row[0], row[1], row[2], row[3] == "1");

    // START TRANSACTION
    // engine = innodb!!!
    q = "start transaction";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "transaction begin failed: " << mysql_error(connection);
        return 0;
    }
    bool regRemoved = removeReg(u);
    if(!regRemoved) // if information not removed from Registration
    {
        qDebug() << "Registration deletion failed... ";
        mysql_query(connection, "rollback");
        return 0;
    }
    QString success = newUser(u, 1);
    if(success != "") // if information added to User
    {
        mysql_query(connection, "commit");
        return 1;
    }
    else // if user creation failed
    {
        mysql_query(connection, "rollback");
        return 0;
    }
}

//Returns 1 if user, password pair exists in validated users
bool dbHandler::loginValidate(const User & p)
{
    MYSQL_RES * result;

    // SELECT * FROM User WHERE username='(username)' and password='(password)'
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

//Returns 1 if register row corresponding to user no longer exists
bool dbHandler::removeReg(const User & u)
{
    MYSQL_RES * result;

    // SELECT * FROM Registration WHERE username='(username)'
    std::string q = "select * from Registration where username='"
        + u.get_username().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "Ruh roh: " << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    bool exists = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    if(!exists) // if information already does not exist in registration
        return 1;

    // DELETE FROM Registration WHERE username='(username)'
    q = "delete from Registration where username='"
        + u.get_username().toStdString() + "'";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "Couldn't delete: " << mysql_error(connection);
        return 0;
    }
    return 1;
}

void dbHandler::cleanReg()
{
    MYSQL_RES * result;
    
    std::string q = "delete from Registration where validTimeout <= NOW()";
    if(mysql_query(connection, q.c_str()))
    {
        qDebug() << "Could not clean";
    }
    result = mysql_store_result(connection);
    mysql_free_result(result);
}
