#ifndef TEXTGUI_H
#define TEXTGUI_H

#include <unordered_map>
#include <functional>

#include <QString>
#include <QDebug>
#include <QSocketNotifier>
#include <QObject>
#include <QTextStream>
#include <QList>
#include <QApplication>

#include "Client.h"

#include "ChatObject.h"

class TextGui : public QObject
{
    Q_OBJECT
public:
    TextGui();
private slots:
    void welcomeMessage();
    void readStdin();
    
private:
    struct Command
    {
        std::function<void(const QStringList&)> call;
        QStringList argNames; 
        QString desc;
    };
    
    void listCommands(const QStringList& args);

    void whoAmI(const QStringList& args);
    void createAcc(const QStringList& args);
    void authenticate(const QStringList& args);
    void login(const QStringList& args);
    void logout(const QStringList& args);
    
    void discover(const QStringList& args);
    void privateMessage(const QStringList& args);
    void viewDMsWith(const QStringList& args);
    void extendMsgHist(const QStringList& args);
    
    void listFriendInfo(const QStringList& args);
    void friendRequest(const QStringList& args);
    void acceptFriend(const QStringList& args);
    void denyFriend(const QStringList& args);
    void removeFriend(const QStringList& args);

    void createGroup(const QStringList & args);
    void groupMessage(const QStringList & args);
private:
    
    
    QTextStream cin;
    QSocketNotifier * reader;
    std::unordered_map<QString, Command> commandMap;
    Client * client;
};

#endif
