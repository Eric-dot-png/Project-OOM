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
bool dbHandler::availUsername(const QString & p)
{
    MYSQL_RES * result;
    
    std::stringstream ss;
    ss << "select * from (select username from User where username='"
       << p.toStdString()
       << "' union select username from Registration where username='"
       << p.toStdString() << "') as T";
    
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    bool avail = mysql_fetch_row(result) == NULL;
    mysql_free_result(result);

    return avail;
}

//Inserts User, and returns 0 if anything goes wrong
QString dbHandler::newUser(const QString & user, const QString & pass,
                           const QString & email, bool perm, bool autoval)
{
    MYSQL_RES * result;
    std::stringstream ss;
    if(autoval) // If bypassing Registration(either testing or already validated)
    {
        // INSERT User(username, password, email, permissions)
        // VALUES('(username)', '(password)', '(email)', permissions)
        ss << "insert User(username, password, email, permissions) values('"
           << user.toStdString() << "', '" << pass.toStdString() << "', '"
           << email.toStdString() << "', " << perm << ")";
        if(mysql_query(connection, ss.str().c_str()))
        {
            qDebug() << "autoval fail " << mysql_error(connection);
            return "";
        }
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
        ss << "insert Registration(username, password, email, permissions, "
           << "validTimeout, code) values('" << user.toStdString() << "', '"
           << pass.toStdString() << "', '" << email.toStdString() << "', "
           << perm << ", '" << timer << "', '" << valcode << "')";
        if(mysql_query(connection, ss.str().c_str()))
        {
            qDebug() << "first false" << mysql_error(connection);
            return "";
        }
        return valcode.c_str(); // returns validation code
    }
}

//returns 1 if user info moved from registration to user
bool dbHandler::emailValidate(const QString & user, const QString & code)
{
    MYSQL_RES * result;
    MYSQL_ROW row;

    // SELECT * FROM Registraion WHERE username='(username)' and code='(code)'
    std::stringstream ss;
    ss << "select * from Registration where username='" << user.toStdString()
       << "' and code='" << code.toStdString() << "'";
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Uh oh:" << mysql_error(connection);
        return 0;
    }

    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    if(row == NULL) // If no user/code combo in table Registration
    {
        qDebug() << "Information not in Registration";
        return 0;
    }
    QString pass = row[1];
    QString email = row[2];
    bool perm = row[3];
    mysql_free_result(result);
    
    // START TRANSACTION
    // engine = innodb!!!
    if(mysql_query(connection, "start transaction"))
    {
        qDebug() << "transaction begin failed: " << mysql_error(connection);
        return 0;
    }
    
    bool regRemoved = removeReg(user);
    if(!regRemoved) // if information not removed from Registration
    {
        qDebug() << "Registration deletion failed... ";
        mysql_query(connection, "rollback");
        return 0;
    }
    
    QString success = newUser(user, pass, email, perm, 1);
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
bool dbHandler::loginValidate(const QString & user, const QString & pass)
{
    MYSQL_RES * result;

    // SELECT * FROM User WHERE username='(username)' and password='(password)'
    std::stringstream ss;
    ss << "select * from User where username='" << user.toStdString()
       << "' and password='" << pass.toStdString() << "'";
    if(mysql_query(connection, ss.str().c_str()))
        return 0;
    result = mysql_store_result(connection);
    bool success = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    return success;
}

//Returns 1 if register row corresponding to user no longer exists
bool dbHandler::removeReg(const QString & user)
{
    MYSQL_RES * result;

    // DELETE FROM Registration WHERE username='(username)'
    std::stringstream ss;
    ss << "delete from Registration where username='" << user.toStdString()
       << "'";
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Couldn't delete: " << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    mysql_free_result(result);
    
    return 1;
}

void dbHandler::cleanReg()
{
    MYSQL_RES * result;
    
    if(mysql_query(connection,
                   "delete from Registration where validTimeout <= NOW()"))
    {
        qDebug() << "Could not clean";
        return;
    }
    result = mysql_store_result(connection);
    mysql_free_result(result);
}

bool dbHandler::storeMessage(const QJsonObject & m)
{
    MYSQL_RES * result;

    std::stringstream ss;
    ss << "insert PrivMessage(receiver, sender, message) values('"
       << m["To"].toString().toStdString() << "', '"
       << m["From"].toString().toStdString() + "', '"
       << m["Message"].toString().toStdString() << "')";
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Could not save new message" << mysql_error(connection);
        return 0;
    }
    result = mysql_store_result(connection);
    mysql_free_result(result);
   
    return 1;
}

QString dbHandler::getMessages(const QString & u1,
                               const QString & u2,
                               int start, int length)
{
    MYSQL_RES * result;
    MYSQL_ROW row;

    std::stringstream ss;
    ss << "select * from (select * from PrivMessage where receiver='"
       << u1.toStdString() << "' and sender='" << u2.toStdString()
       << "' union select * from PrivMessage where receiver='"
       << u2.toStdString() << "' and sender='" << u1.toStdString()
       << "') as T where not deleted order by sentAt desc limit "
       << start << ", " << length;
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "DB failed to retrieve messages";
        return "";
    }

    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    QStringList res;
    while(row != NULL)
    {
        res.push_back(QJsonDocument(QJsonObject({{"To", row[0]},
                                                  {"From", row[1]},
                                                  {"Timestamp", row[2]},
                                                  {"Message", row[3]}})
                ).toJson());
        row = mysql_fetch_row(result);
    }
    return res.join(":;:");
}

bool dbHandler::addFriend(const QString & u1, const QString & u2)
{
    MYSQL_RES * result;

    std::stringstream ss;
    ss << "select * from Friends where (u1='" << u1.toStdString()
       << "' and u2='" << u2.toStdString() << "') or (u1='"
       << u2.toStdString() << "' and u2='" << u1.toStdString() << "')";
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Failed select 1 in addFriend";
        return 0;
    }
    result = mysql_store_result(connection);
    if(mysql_fetch_row(result) == NULL)
    {
        mysql_free_result(result);
        ss.flush();
        ss << "insert Friends(u1, u2) values('" << u1.toStdString() << "', '"
           << u2.toStdString() << "'";
        if(mysql_query(connection, ss.str().c_str()))
        {
            qDebug() << "Failed to insert Friendship";
            return 0;
        }
        result = mysql_store_result(connection);
    }

    mysql_free_result(result);
    return 1;
}

QStringList dbHandler::getFriendslist(const QString & user)
{
    MYSQL_RES * result;
    MYSQL_ROW row;
    QStringList ret;

    std::stringstream ss;
    ss << "select * from (select u2 from Friends where u1='"
       << user.toStdString() << "' union select u1 from Friends where u2='"
       << user.toStdString() << "') as T";
    if(mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "select 1 fail in getFriendslist()";
        return {};
    }
    
    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    while(row != NULL)
    {
        ret.push_back(row[0]);
        row = mysql_fetch_row(result);
    }
    mysql_free_result(result);

    return ret;
}
