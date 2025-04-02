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

#include "Client.h"

class TextGui : public QObject
{
    Q_OBJECT
public:
    TextGui();

    void whoAmI(const QStringList& args);
    void listCommands(const QStringList& args={});
    void createAcc(const QStringList& args);
    void authenticate(const QStringList& args);
    void login(const QStringList& args);
    void discover(const QStringList& args);
    void privateMessage(const QStringList& args);
    void friendRequest(const QStringList& args);
    void acceptFriend(const QStringList& args);
    void denyFriend(const QStringList& args);
    void listFriendInfo(const QStringList& args);
    // void removeFriend(const QStringList& args);
    // void extendMessageHistory(const QStringList& args);
                                       
private slots:
    void readStdin();
private:
    struct Command
    {
        std::function<void(const QStringList&)> call;
        QStringList argNames; 
        QString desc;
    };
    
private:
    QTextStream cin;
    QSocketNotifier * reader;
    std::unordered_map<QString, Command> commandMap;
    Client * client;
};

#endif
