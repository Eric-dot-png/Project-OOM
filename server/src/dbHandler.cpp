#include "dbHandler.h"

// Static pointer to hold the single instance of dbHandler (Singleton pattern)
dbHandler * dbHandler::dbHandler_ = NULL;

// Constructor: initializes the MySQL connection. If connection fails, prints the error.
dbHandler::dbHandler()
{
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, 
                                    "127.0.0.1", // Database IP address or hostname
                                    "root",      // Database username (change as needed)
                                    "root",      // Database password (change as needed)
                                    "DSDB",      // Database name
                                    0,           // Port (0 for default)
                                    0,           // Socket (0 for default)
                                    0);          // Client flags
    if (connection == NULL)
        printf(mysql_error(&mysql));
}

// Destructor: closes the MySQL connection.
dbHandler::~dbHandler()
{
    mysql_close(connection);
}

// Retrieves the single instance of dbHandler. If it doesn't exist, creates it.
dbHandler * dbHandler::GetInstance()
{
    if (dbHandler_ == NULL)
        dbHandler_ = new dbHandler();
    return dbHandler_;
}

// Destroys the dbHandler singleton instance and frees its memory.
void dbHandler::destroyInstance()
{
    if (dbHandler_ != NULL)
    {
        delete dbHandler_;
        dbHandler_ = NULL;
    }
}

// Checks if a username is available by ensuring it's not present in either the User or Registration table.
bool dbHandler::availUsername(const QString & p)
{
    MYSQL_RES * result;
    
    std::stringstream ss;
    // Create an SQL query that checks both the User table and the Registration table
    // for any rows matching the given username.
    ss << "select * from (select username from User where username='"
       << p.toStdString()
       << "' union select username from Registration where username='"
       << p.toStdString() << "') as T";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << mysql_error(connection);
        return 0;
    }
    
    // Store the results and check if any row was returned
    result = mysql_store_result(connection);
    bool avail = mysql_fetch_row(result) == NULL;
    mysql_free_result(result);

    return avail;
}

// Checks if a user exists in the User table.
bool dbHandler::userExists(const QString & user)
{
    MYSQL_RES * result;

    std::stringstream ss;
    ss << "select * from User where username='" << user.toStdString() << "'";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Error in selecting from User";
        return 0;
    }
    result = mysql_store_result(connection);
    bool exists = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);

    return exists;
}

// Inserts a new user record into either the User table (validated immediately) or the Registration table (awaiting email validation).
// Returns either the newly generated validation code for email-based signups or an empty string if something fails.
QString dbHandler::newUser(const QString & user, const QString & pass,
                           const QString & email, bool perm, bool autoval)
{
    std::stringstream ss;
    if (autoval) // If bypassing Registration (e.g., already validated or in testing)
    {
        ss << "insert User(username, password, email, permissions) values('"
           << user.toStdString() << "', '"
           << pass.toStdString() << "', '"
           << email.toStdString() << "', " << perm << ")";
        if (mysql_query(connection, ss.str().c_str()))
        {
            qDebug() << "autoval fail " << mysql_error(connection);
            return "";
        }
        // If successful and autoval is true, return "validated" to indicate success
        return "validated";
    }
    else // If registration requires validation
    {
        // Insert into Registration with a random 6-digit validation code and a timeout 30 minutes from now
        std::string timer = QDateTime::currentDateTime().addSecs(1800).toString("yyyy-MM-dd hh:mm:ss").toStdString();
        std::string valcode;
        for(int i = 0; i < 6; i++)
            valcode += '0' + rand() % 10;

        ss << "insert Registration(username, password, email, permissions, validTimeout, code) values('"
           << user.toStdString() << "', '"
           << pass.toStdString() << "', '"
           << email.toStdString() << "', "
           << perm << ", '" << timer << "', '" << valcode << "')";
        if (mysql_query(connection, ss.str().c_str()))
        {
            qDebug() << "first false" << mysql_error(connection);
            return "";
        }
        // Return the generated validation code so the user can confirm their email
        return valcode.c_str();
    }
}

// Validates a user's email by matching username and code in the Registration table,
// then transferring that record from Registration to the User table if it's correct.
bool dbHandler::emailValidate(const QString & user, const QString & code)
{
    MYSQL_RES * result;
    MYSQL_ROW row;

    // Check if the user and code combo exist in Registration
    std::stringstream ss;
    ss << "select * from Registration where username='" << user.toStdString()
       << "' and code='" << code.toStdString() << "'";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Uh oh:" << mysql_error(connection);
        return 0;
    }

    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    if (row == NULL) // If no user/code combo was found
    {
        qDebug() << "Information not in Registration";
        mysql_free_result(result);
        return 0;
    }
    QString pass = row[1];
    QString email = row[2];
    bool perm = row[3];
    mysql_free_result(result);

    // Begin a MySQL transaction
    if (mysql_query(connection, "start transaction"))
    {
        qDebug() << "transaction begin failed: " << mysql_error(connection);
        return 0;
    }
    
    // Remove this user from Registration
    bool regRemoved = removeReg(user);
    if (!regRemoved)
    {
        qDebug() << "Registration deletion failed... ";
        mysql_query(connection, "rollback");
        return 0;
    }
    
    // Insert user data into the User table
    QString success = newUser(user, pass, email, perm, 1);
    if (success != "") // If success is non-empty, user creation succeeded
    {
        mysql_query(connection, "commit");
        return 1;
    }
    else
    {
        mysql_query(connection, "rollback");
        return 0;
    }
}

// Checks if the (username, password) pair exists in the User table (meaning the user is validated).
bool dbHandler::loginValidate(const QString & user, const QString & pass)
{
    MYSQL_RES * result;

    std::stringstream ss;
    ss << "select * from User where username='" << user.toStdString()
       << "' and password='" << pass.toStdString() << "'";
    if (mysql_query(connection, ss.str().c_str()))
        return 0;

    result = mysql_store_result(connection);
    bool success = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);
    return success;
}

// Deletes a user's row from Registration, typically used after validating or canceling registration.
bool dbHandler::removeReg(const QString & user)
{
    std::stringstream ss;
    ss << "delete from Registration where username='" << user.toStdString() << "'";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Couldn't delete: " << mysql_error(connection);
        return 0;
    }
    
    return 1;
}

// Cleans up expired registration records (whose validTimeout <= current time).
void dbHandler::cleanReg()
{
    if (mysql_query(connection,
                    "delete from Registration where validTimeout <= NOW()"))
    {
        qDebug() << "Could not clean";
        return;
    }
}

// Stores a private message in the PrivMessage table.
bool dbHandler::storeMessage(const QJsonObject & m)
{
    std::stringstream ss;
    ss << "insert PrivMessage(receiver, sender, message) values('"
       << m["To"].toString().toStdString() << "', '"
       << m["From"].toString().toStdString() << "', '"
       << m["Message"].toString().toStdString() << "')";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Could not save new message" << mysql_error(connection);
        return 0;
    }
   
    return 1;
}

// Retrieves a list of messages between two users, in descending order of timestamp,
// returning them in a specific format, separated by ':;:'.
QString dbHandler::getMessages(const QString & u1,
                               const QString & u2,
                               int start, int length)
{
    MYSQL_RES * result;
    MYSQL_ROW row;

    // Query selects all messages (from either direction) that aren't marked deleted
    // and orders them descending by sentAt. The limit is for pagination or partial retrieval.
    std::stringstream ss;
    ss << "select * from (select * from PrivMessage where receiver='"
       << u1.toStdString() << "' and sender='" << u2.toStdString()
       << "' union select * from PrivMessage where receiver='"
       << u2.toStdString() << "' and sender='" << u1.toStdString()
       << "') as T where not deleted order by sentAt desc limit "
       << start << ", " << length;

    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "DB failed to retrieve messages";
        return "";
    }

    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    QStringList res;
    // Collect each row's data in JSON form, then push to a QStringList.
    while (row != NULL)
    {
        res.push_back(QJsonDocument(
            QJsonObject({
                {"To",        row[0]},
                {"From",      row[1]},
                {"Timestamp", row[2]},
                {"Message",   row[3]}
            })
        ).toJson());
        row = mysql_fetch_row(result);
    }
    mysql_free_result(result);
    // Messages are joined with ':;:' delimiter and returned
    return res.join(":;:");
}

// Adds a friend request record in the FriendRequest table.
bool dbHandler::addFriendRequest(const QString & from, const QString & to)
{
    std::stringstream ss;
    ss << "insert FriendRequest(sender, receiver) values('"
       << from.toStdString() << "', '" << to.toStdString() << "')";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Couldn't add friend request";
        return 0;
    }

    return 1;
}

// Removes a friend request record from the FriendRequest table.
bool dbHandler::removeFriendRequest(const QString & from, const QString & to)
{
    std::stringstream ss;
    ss << "delete from FriendRequest where sender='" << from.toStdString()
       << "' and receiver='" << to.toStdString() << "'";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Could not remove friend request";
        return 0;
    }
    return 1;
}

// Retrieves a list of friend requests for a given user (where the user is the receiver).
QStringList dbHandler::getFriendRequests(const QString & u)
{
    MYSQL_RES * result;
    MYSQL_ROW row;
    QStringList ret;

    std::stringstream ss;
    ss << "select * from FriendRequest where receiver='" << u.toStdString() << "'";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Failed to retrieve friend requests";
        return {};
    }
    
    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    // For each row, the 'sender' is row[0].
    while (row != NULL)
    {
        ret.push_back(row[0]);
        row = mysql_fetch_row(result);
    }
    mysql_free_result(result);

    return ret;
}

// Adds a friendship record between two users, after removing any outstanding friend requests
// in a transaction to ensure data integrity.
bool dbHandler::addFriend(const QString & u1, const QString & u2)
{
    // Start a database transaction
    if (mysql_query(connection, "start transaction"))
    {
        qDebug() << "Could not start transaction";
        return 0;
    }

    // Remove any friend requests in either direction
    if (!removeFriendRequest(u1, u2))
        return 0;
    if (!removeFriendRequest(u2, u1))
        return 0;
    
    // Insert a friendship record
    std::stringstream ss;
    ss << "insert Friends(u1, u2) values('" << u1.toStdString()
       << "', '" << u2.toStdString() << "')";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Failed to insert Friendship";
        mysql_query(connection, "rollback");
        return 0;
    }
    
    // Commit the transaction if all statements succeeded
    mysql_query(connection, "commit");
    return 1;
}

// Removes a friendship record between two users.
bool dbHandler::removeFriend(const QString & u1, const QString & u2)
{
    std::stringstream ss;
    // Deletes the record if it matches (u1, u2) or (u2, u1)
    ss << "delete from Friends where (u1='" << u1.toStdString()
       << "' and u2='" << u2.toStdString() << "') or (u1='"
       << u2.toStdString() << "' and u2='" << u1.toStdString() << "')";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "failed to delete friendship";
        return 0;
    }
    return 1;
}

// Retrieves a user's friend list by looking up all pairs in the Friends table.
QStringList dbHandler::getFriendslist(const QString & user)
{
    MYSQL_RES * result;
    MYSQL_ROW row;
    QStringList ret;

    std::stringstream ss;
    // Query gets friends from both directions (u1 or u2 as 'user')
    ss << "select * from (select u2 from Friends where u1='"
       << user.toStdString() << "' union select u1 from Friends where u2='"
       << user.toStdString() << "') as T";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "select 1 fail in getFriendslist()";
        return {};
    }
    
    result = mysql_store_result(connection);
    row = mysql_fetch_row(result);
    // Each row is one friend in the list
    while (row != NULL)
    {
        ret.push_back(row[0]);
        row = mysql_fetch_row(result);
    }
    mysql_free_result(result);

    return ret;
}

// Checks if two users are friends by looking up records in the Friends table.
bool dbHandler::areFriends(const QString & u1, const QString & u2)
{
    MYSQL_RES * result;

    std::stringstream ss;
    ss << "select * from Friends where (u1='" << u1.toStdString()
       << "' and u2='" << u2.toStdString() << "') or (u1='"
       << u2.toStdString() << "' and u2='" << u1.toStdString() << "')";
    if (mysql_query(connection, ss.str().c_str()))
    {
        qDebug() << "Couldn't search friends\n";
        return 0;
    }
    result = mysql_store_result(connection);
    bool res = mysql_fetch_row(result) != NULL;
    mysql_free_result(result);

    return res;
}