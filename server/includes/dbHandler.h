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
      USER CREATION/VALIDATION/DELETION FUNCTIONS
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
    //Removes user, returns 0 if something went wrong
    //bool deleteUser(const QString & u);
    //Returns 1 if login credentials match a user
    bool loginValidate(const QString & user, const QString & pass);
    //Returns 1 if registration credentials are removed
    bool removeReg(const QString & user);
    //Flushes out all expired registrations
    void cleanReg();

    /*--------------------------------------------
      MESSAGING FUNCTIONS
    --------------------------------------------*/
    //returns 1 if stored private message
    bool storeMessage(const QJsonObject &);
    //returns 1 if stored group message
    bool storeGroupMessage(const QJsonObject &);
    //returns list of (length) messages beginning at start(0 is most recent message)
    QJsonArray getMessages(const QString & u1, const QString & u2,
                           int start = 0, int length = 50);
    //same as getMessages but for groups
    QJsonArray getGroupMessages(const QString & owner, const QString & name,
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

    /*--------------------------------------------
      BLOCK FUNCTIONS
    --------------------------------------------*/
    bool addBlock(const QString & u, const QString & blocked);
    bool removeBlock(const QString & u, const QString & blocked);
    QStringList getBlockList(const QString & u);
    
    /*--------------------------------------------
      GROUP FUNCTIONS
    --------------------------------------------*/
    //returns group id
    int getGroupId(const QString & owner, const QString & name);
    //returns 1 if group name is available
    bool availGroup(const QString & u, const QString & name);
    //returns 1 if new Group stored and members added
    bool newGroup(const QString & u, const QString & name,
                  const QStringList & members);
    //returns 1 if member added to group
    bool addGroupMember(int groupId, const QString & mem);
    bool addGroupMember(const QString & owner, const QString & name,
                        const QString & mem);
    //returns 1 if group deleted
    //bool deleteGroup(const QString & u, const QString & name);
    //Returns groups
    QJsonArray getGroups(const QString & u);
    //Returns list of group members
    QStringList getGroupMembers(const QString & owner, const QString & name);
    
private:
    dbHandler();
    ~dbHandler();
    
    MYSQL * connection, mysql;
    static dbHandler * dbHandler_;
};  

#endif
