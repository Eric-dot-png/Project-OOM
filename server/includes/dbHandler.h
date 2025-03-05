#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtCore>
#include <mysql.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <QDateTime>

class dbHandler
{
public:
    static dbHandler * GetInstance();
    static void destroyInstance();

    /*--------------------------------------------
      USER CREATION/VALIDATION FUNCTIONS
    --------------------------------------------*/
    //Checks if username is already used
    bool availUsername(const QString &);
    //Checks if a user exists
    bool userExists(const QString &);
    //Adds new user to db, returns 0 if failed
    QString newUser(const QString & user, const QString & pass,
                    const QString & email, bool perm = 0, bool autoval = 0);
    //Validates user, returns 0 if something went wrong
    bool emailValidate(const QString & user, const QString & code);
    //Returns 1 if login credentials match a user
    bool loginValidate(const QString & user, const QString & pass);
    //Returns 1 if registration credentials are removed
    bool removeReg(const QString & user);
    //Flushes out all expired registrations
    void cleanReg();

    /*--------------------------------------------
      MESSAGING FUNCTIONS
    --------------------------------------------*/
    //returns 1 if stored messages updated
    bool storeMessage(const QJsonObject &);
    //returns list of (length) messages separated by ":;:" beginning at start(0 is most recent message)
    QString getMessages(const QString & u1, const QString & u2,
                        int start = 0, int length = 50);

    /*--------------------------------------------
      FRIEND FUNCTIONS
    --------------------------------------------*/
    //returns 1 if friend request created
    bool addFriendRequest(const QString & from, const QString & to);
    //returns 1 if friend request removed
    bool removeFriendRequest(const QString & from, const QString & to);
    //returns pending friend requests
    QStringList getFriendRequests(const QString & u);
    //returns 1 if friendship created
    bool addFriend(const QString & u1, const QString & u2);
    //returns 1 if friendship removed
    bool removeFriend(const QString & u1, const QString & u2);
    //returns friendlist(WOW!)
    QStringList getFriendslist(const QString & user);
    //returns 1 if friendship exists
    bool areFriends(const QString & u1, const QString & u2);
    
private:
    dbHandler();
    ~dbHandler();
    
    MYSQL * connection, mysql;
    static dbHandler * dbHandler_;
};  

#endif
