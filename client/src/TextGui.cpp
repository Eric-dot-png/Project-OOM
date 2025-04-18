#include "TextGui.h"

TextGui::TextGui()
    : reader(new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this)),
      client(Client::getInstance()),
      cin(QTextStream(stdin))
{
    client->connectToServer(QHostAddress(), 0);
    
    connect(reader, &QSocketNotifier::activated, this, &TextGui::readStdin);

    commandMap["whoami"] = {
        std::bind(&TextGui::whoAmI, this, std::placeholders::_1),
        {}, "Shows the current instance's account username"
    };
    
    commandMap["lc"] = {
        std::bind(&TextGui::listCommands, this, std::placeholders::_1),
        {}, "Shows all available cmds"
    };

    commandMap["create"] = {
        std::bind(&TextGui::createAcc, this, std::placeholders::_1),
        {"Username", "Password1", "Password2", "Email"},
        "Attempts to create an account. If accepted, must verify with code. (See auth)"
    };

    commandMap["auth"] = {
        std::bind(&TextGui::authenticate, this, std::placeholders::_1),
        {"Code"}, "Verify an account by using an authentication code sent to your email"
    };

    commandMap["login"] = {
        std::bind(&TextGui::login, this, std::placeholders::_1),
        {"Username", "Password"}, "Log in to an existing account"
    };

    commandMap["logout"] = {
        std::bind(&TextGui::logout, this, std::placeholders::_1),
        {}, "Logs out from the current account"
    };
    
    commandMap["disc"] = {
        std::bind(&TextGui::discover, this, std::placeholders::_1),
        {"Username"}, "Discovers an existing private message history with another user"
    };
    
    commandMap["pm"] = {
        std::bind(&TextGui::privateMessage, this, std::placeholders::_1),
        {"Username", "Message"}, "Send a private message to a discovered user"
    };

    commandMap["fr"] = {
        std::bind(&TextGui::friendRequest, this, std::placeholders::_1),
        {"Username"}, "Request another user to be friends."
    };

    commandMap["afr"] = {
        std::bind(&TextGui::acceptFriend, this, std::placeholders::_1),
        {"Username"}, "Accepts another user's friend request"
    };

    commandMap["dfr"] = {
        std::bind(&TextGui::denyFriend, this, std::placeholders::_1),
        {"Username"}, "Denies another user's friend request"
    };

    commandMap["vfi"] = {
        std::bind(&TextGui::listFriendInfo, this, std::placeholders::_1),
        {}, "Shows all Friend Requests and Friends"
    };

    commandMap["vpm"] = {
        std::bind(&TextGui::viewDMsWith, this, std::placeholders::_1),
        {"User"}, "Shows the private message history with user."
    };

    commandMap["rmf"] = {
        std::bind(&TextGui::removeFriend, this, std::placeholders::_1),
        {"Friend"}, "Removes the friend from the friendlist"
    };

    commandMap["extend"] = {
        std::bind(&TextGui::extendMsgHist, this, std::placeholders::_1),
        {"User"}, "Asks the server for more pms with User."
    };

    commandMap["cgrp"] = {
        std::bind(&TextGui::createGroup, this, std::placeholders::_1),
        {"Group Name", "Members"},
        "Requests to create a group with Members(separate by ,)"
    };

    commandMap["mgrp"] = {
        std::bind(&TextGui::groupMessage, this, std::placeholders::_1),
        {"Group Owner", "Group Name", "Message"}, "Sends a message to a group"
    };

    commandMap["block"] = {
        std::bind(&TextGui::blockUser, this, std::placeholders::_1),
        {"Username"}, "Blocks the user with Username"
    };
    
    commandMap["unblock"] = {
        std::bind(&TextGui::unblockUser, this, std::placeholders::_1),
        {"Username"}, "unblocks the user with Username"
    };

    commandMap["vblocklist"] = {
        std::bind(&TextGui::viewBlocklist, this, std::placeholders::_1),
        {}, "Shows blocklist"
    };

    commandMap["vgrp"] = {
        std::bind(&TextGui::viewGroup, this, std::placeholders::_1),
        {"Group Owner", "Group Name"}, "Shows group message history"
    };

    commandMap["extendGroup"] = {
        std::bind(&TextGui::extendGroup, this, std::placeholders::_1),
        {"Group Owner", "Group Name"}, "Extends group message history"
    };

    commandMap["leaveGroup"] = {
        std::bind(&TextGui::leaveGroup, this, std::placeholders::_1),
        {"Group Owner", "Group Name"}, "Removes self from group"
    };

    commandMap["addGroupMember"] = {
        std::bind(&TextGui::addGroupMember, this, std::placeholders::_1),
        {"Group Owner", "Group Name", "User"}, "Adds user to a group(must be owner)"
    };

    commandMap["transferGroup"] = {
        std::bind(&TextGui::transferGroup, this, std::placeholders::_1),
        {"Group Owner", "Group Name", "User"}, "Transfers group ownership to user(must be owner)"
    };

    commandMap["deleteGroup"] = {
        std::bind(&TextGui::deleteGroup, this, std::placeholders::_1),
        {"Group Owner", "Group Name"}, "deletes the group(must be owner)"
    };
    
    // welcome message
    connect(client,&Client::connectedToServer,this,&TextGui::welcomeMessage);
}

void TextGui::welcomeMessage()
{
    QTextStream cout(stdout);
    cout << "Welcome to OOM (Text Edition)!\n";
    cout << "Type \"lc\" (without quotes) to get started\n\n";
    disconnect(client, &Client::connectedToServer, this,
               &TextGui::welcomeMessage);
}

void TextGui::readStdin()
{
    QString input = cin.readLine();
    if (input.isNull())
    {
        // eof
        QCoreApplication::quit();
        return;
    }
    
    QStringList line = input.simplified().split(' ');
   
    QTextStream cout(stdout);
    QString cmd = line[0];
    auto search = commandMap.find(cmd);
    if (search != commandMap.end())
    {
        Command c = search->second;
        qDebug() << "Valid Command.";
        QStringList args = {};
        if (line.size() > 0) args = line.mid(1,line.size()-1);
        for (int i=args.size();i<c.argNames.size();++i)
        {
            cout << ">> Enter " << c.argNames[i] << ": ";
            cout.flush();
            args.append(cin.readLine());
        }
        qDebug() << "Arguments: " << args;
        c.call(args);
    }
    else
    {
        cout << "Unknown Command.\n";
    }
}

void TextGui::listCommands(const QStringList& args)
{
    qDebug() << "Listing Commands...";
    QTextStream cout(stdout);
    cout << "Available Commands:\n";
    for (const auto &[name, cmd] : commandMap)
    {
        cout << "  " << name << " - " << "Description: " << cmd.desc << '\n'
             << "  " << QString(' ').repeated(name.size()) << "   Arguments: "
             << (cmd.argNames.size() == 0 ? "None" : cmd.argNames.join(", "))
             << '\n';
    }
}

void TextGui::whoAmI(const QStringList& args)
{
    qDebug() << "Displaying whoami...";

    QTextStream cout(stdout);
    cout << "Current Account: " << client->getUser().get_username() << '\n';
}


void TextGui::createAcc(const QStringList& args)
{
    qDebug() << "Creating Account...";

    // asuming pwd1 == pwd2
    client->createAccount(User(args[0],args[1],args[3]));
    
}

void TextGui::authenticate(const QStringList& args)
{
    qDebug() << "Authenticating Account...";

    client->submitAuthCode(args[0]);
}

void TextGui::login(const QStringList& args)
{
    qDebug() << "Logging in...";

    client->login(User(args[0], args[1]));
}

void TextGui::logout(const QStringList& args)
{
    qDebug() << "Logging out";

    client->logout();
}

void TextGui::discover(const QStringList& args)
{
    client->discover(User(args[0]));
}

void TextGui::privateMessage(const QStringList& args)
{
    client->privateMessage(User(args[0]), args[1]);
}

void TextGui::viewDMsWith(const QStringList& args)
{
    QTextStream cout(stdout);
    try
    {
        const QList<Message> * dms = &client->getDMsWith(args[0])->allMessages();
        for (int i=dms->size()-1;i>=0;--i)
        {
            const Message * m = &((*dms)[i]);
            cout << m->get_sender() << ": "
                 << m->get_msg() << '\n';
        }    
    }
    catch (const std::runtime_error & error)
    {
        qDebug() << "WARNING: Dont forget to discover, no msgs found.";
    }
}

void TextGui::extendMsgHist(const QStringList& args)
{
    qDebug() << "extending...";
    client->extendMessageHistory(User(args[0]));
}

void TextGui::listFriendInfo(const QStringList& args)
{
    QTextStream cout(stdout);

    User * current = &client->getUser();

    cout << "Friend Requests:\n";
    for (const QString& fr : current->getFriendRequestList())
        cout << "   " << fr << '\n';

    cout << "Friends:\n";
    for (const QString& f : current->getFriendsList())
        cout << "   " << f << '\n';    
}

void TextGui::friendRequest(const QStringList& args)
{
    client->friendRequest(User(args[0]));
}

void TextGui::acceptFriend(const QStringList& args)
{
    client->acceptFriend(User(args[0]));
}

void TextGui::denyFriend(const QStringList& args)
{
    client->denyFriend(User(args[0]));
}

void TextGui::removeFriend(const QStringList& args)
{
    client->removeFriend(User(args[0]));
}

void TextGui::blockUser(const QStringList& args)
{
    client->block(User(args[0]));
}

void TextGui::unblockUser(const QStringList& args)
{
    client->unblock(User(args[0]));
}

void TextGui::viewBlocklist(const QStringList & args)
{
    QTextStream cout(stdout);
    std::unordered_set<QString> blocklist = client->getBlockList().blocked();
    for(const QString & b : blocklist)
        cout << b << '\n';
}

void TextGui::createGroup(const QStringList& args)
{
    QStringList members = args[1].split(",");
    client->createGroup(args[0], members);
}

void TextGui::groupMessage(const QStringList & args)
{
    client->messageGroup(args[0], args[1], args[2]);
}

void TextGui::viewGroup(const QStringList & args)
{
    QTextStream cout(stdout);
    const ChatObject * g = client->getGroupHistory(args[0], args[1]);
    if(g == NULL)
        qDebug() << "Fetching history, try again momentarily";
    else
    {
        qDebug() << "Displaying History";
        const QList<Message> messages = g->allMessages();
        for (int i=messages.size()-1;i>=0;--i)
        {
            const Message m = messages[i];
            cout << m.get_sender() << ": "
                 << m.get_msg() << '\n';
        }
    }
}

void TextGui::extendGroup(const QStringList & args)
{
    client->extendGroupHistory(args[0], args[1]);
}

void TextGui::leaveGroup(const QStringList & args)
{
    client->leaveGroup(args[0], args[1]);
}

void TextGui::addGroupMember(const QStringList & args)
{
    client->addGroupMember(args[0], args[1], args[2]);
}

void TextGui::transferGroup(const QStringList & args)
{
    client->transferGroupOwnership(args[0], args[1], args[2]);
}

void TextGui::deleteGroup(const QStringList & args)
{
    client->deleteGroup(args[0], args[1]);
}
