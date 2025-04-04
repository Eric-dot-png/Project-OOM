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

    
    // welcome message
    connect(client, &Client::connectedToServer, this,
            &TextGui::welcomeMessage);
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
    QStringList line = cin.readLine().simplified().split(' ');
    QTextStream cout(stdout);
    QString input = line[0];
    auto search = commandMap.find(input);
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
