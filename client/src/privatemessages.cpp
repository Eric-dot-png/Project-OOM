//Name: Alex Franke
//File: privateMessages.cpp
//OOM Project

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextBrowser>

#include "OOMTextBrowser.h"
#include "privatemessages.h"
#include "ui_privatemessages.h"
#include "User.h"
#include "message.h"

PrivateMessages::PrivateMessages(QWidget *parent)
    : OOMWidget(parent), ui(new Ui::PrivateMessages),
      currentlyMessaging("","",""),
    messagingList(new DMListModel(this))
{
    ui->setupUi(this);
    ui->friendNameLabel->clear();
    client->getFriendsList(client->getUser());

    ui->friendView->setModel(messagingList);
    messagingList->addUserToDMList(User("Test"));
    
    //This allows the textbox to detect the enter key
    enterFilter = new EnterKeyFilter(this);
    ui->textEdit->installEventFilter(enterFilter);

    //Connect enter key
    connect(enterFilter, &EnterKeyFilter::enterPressed, this, &PrivateMessages::onEnterKeyPressed);

    //Detect enter key
    connect(ui->searchUserTextbox, &QLineEdit::returnPressed, this, &PrivateMessages::searchUser);

    //Show message when there is an incoming message
    connect(client, &Client::recievedDM, this, &PrivateMessages::receivedMessage);


    //If searching for a user fails
    connect(client, &Client::discoverUserFail, this, [=](const QString& username){
        ui->userNotFoundLabel->setText("User " + username +  " not found!");
        ui->friendNameLabel->clear();
    });

    //If searching for a user succeeds
    connect(client, &Client::discoverUserSucceed, this, [=](const QString& username, const QList<QJsonObject> & messageJsonList){

        ui->userNotFoundLabel->clear();
        ui->friendNameLabel->setText("Now messaging: " + username);
        currentlyMessaging = User(username);
        //qDebug() << currentlyMessaging.get_username();

        //Add user to messagingList
        messagingList->addUserToDMList(currentlyMessaging);

        //qDebug() << messageJsonList;

        for (auto it = messageJsonList.rbegin(); it != messageJsonList.rend(); ++it)
        {
            QJsonObject obj = *it;
            QString to = obj["To"].toString();
            QString from = obj["From"].toString();
            QString msg = obj["Message"].toString();

            //qDebug() << to + ' ' + from + ' ' + msg;

            ui->textBrowser->appendMessage(Message(from, to, msg), 1);
            messagingList->messageReceived(currentlyMessaging, Message(from, to, msg));
        }
        showAddFriendButton();
    });


    //Retrieve friends list and friend requests,

    connect(client, &Client::sendFriendRequestList, this, [=](const QString usr, const QStringList& list)
    {

        if (usr == currentlyMessaging.get_username())
            currentlyMessaging.setFriendRequestList(list);
        else if (usr == client->getUser().get_username())
            client->getUser().setFriendRequestList(list);
    });

    connect(client, &Client::sendFriendList, this, [=](const QString& usr, const QStringList& list) {
        //qDebug() << "Setting friendlist.";
        if (usr == currentlyMessaging.get_username())
        {
            //qDebug() << "Setting currentlyMessaging friend list as: " << list;
            currentlyMessaging.setFriendList(list);
        }
        else if (usr == client->getUser().get_username())
        {
            qDebug() << "Setting client friendlist as: " << list;
            client->getUser().setFriendList(list);
        }
    });


    connect(ui->denyButton, &QPushButton::clicked, this, [=](){

        //qDebug() << "Friends list: \n" << client->getUser().getFriendsList();
        for (const QString& frnd : client->getUser().getFriendsList())
            qDebug() << frnd << '\n';

    });

    connect(ui->addFriendButton, &QPushButton::clicked, this, &PrivateMessages::sendFriendRequest);

    connect(ui->acceptButton, &QPushButton::clicked, this, [=](){
        client->acceptFriend(User(ui->friendRequestComboBox->currentText()));

    });

    connect(ui->friendView, &QListView::clicked, this, &PrivateMessages::openDM);

    ui->currentUser->setText(client->getUser().get_username());

    connect(client, &Client::recievedFriendRequest, this, [=](const QString& from)
    {
        ui->friendRequestComboBox->addItem(from);
    });

    for (const QString& usr : client->getUser().getFriendRequestList())
        ui->friendRequestComboBox->addItem(usr);

    //populate friend combobox when retrieved...


}

PrivateMessages::~PrivateMessages()
{
    delete ui;
}

void PrivateMessages::showAddFriendButton()
{
    if (client->getUser().getFriendsList().contains(currentlyMessaging.get_username()))
        ui->addFriendButton->hide();
    else ui->addFriendButton->show();
}

void PrivateMessages::searchUser()
{
    ui->textBrowser->clearHistory();
    ui->textBrowser->clear();
    //qDebug() << "Searching for user: " << ui->searchUserTextbox->text();
    User u = User(ui->searchUserTextbox->text());

    if (u.get_username() == client->getUser().get_username()) return;
    if (u.get_username() == currentlyMessaging.get_username())
    {
        //figure out how to open message from list
        //without clicking on them
        return;
    }

    ui->userNotFoundLabel->setText("Currently searching for: " + u.get_username());
    client->discover(u);

}

QString PrivateMessages::formatOtherMessage()
{
    //get from client/server
    QString otherMsg = "";

    QString user = currentlyMessaging.get_username();

    return "<b>" + user + "</b>:<br>" + otherMsg;
}

QString PrivateMessages::formatClientMessage()
{
    QString msg = ui->textEdit->toMarkdown();
    QString user = client->getUser().get_username();

    return "<b>" + user + "</b>:<br>" + msg;

}

void PrivateMessages::onEnterKeyPressed()
{
    //qDebug() << "Sending message: " << ui->textEdit->toPlainText();

    QString fullMsg = formatClientMessage();
    QString msgContent = ui->textEdit->toPlainText();

    Message msg = Message(client->getUser().get_username(), currentlyMessaging.get_username(), msgContent);

    QString user = currentlyMessaging.get_username();

    ui->textBrowser->appendMessage(Message(client->getUser().get_username(), currentlyMessaging.get_username(), msgContent), 0);

    ui->textEdit->clear();
    messagingList->messageReceived(user, msg);
    client->privateMessage(user, msg.get_msg());


}

void PrivateMessages::receivedMessage(QString from, QString amsg)
{
    Message msg = Message(User(from).get_username(), client->getUser().get_username(), amsg);
    if (from == currentlyMessaging.get_username())
    {
        ui->textBrowser->appendMessage(msg, 0);
        messagingList->messageReceived(User(from), msg);
    }
    else
    {
        //if user doesnt have current messaging window open
        messagingList->messageReceived(User(from), msg);
    }


}

void PrivateMessages::openDM(const QModelIndex &index)
{
    if (!index.isValid())
    {
        qDebug() << "Invalid index in openDM.";
        return;
    }

    QVariant userData = index.data(Qt::UserRole);
    if (!userData.isValid())
    {
        qDebug() << "Invalid user data in openDM.";
        return;
    }

    currentlyMessaging = userData.value<User>();
    //qDebug() << "Switching to DM with: " << currentlyMessaging.get_username();


    ui->friendNameLabel->setText("Now messaging: " + currentlyMessaging.get_username());
    showAddFriendButton();
    QList<Message> history = messagingList->getMessageHistory(currentlyMessaging);
    ui->textBrowser->clear();
    for (const Message &msg : history)
    {
        ui->textBrowser->appendMessage(msg, 1);
    }
}

void PrivateMessages::sendFriendRequest()
{
    qDebug() << "Friend request button clicked.";
    //disgustingly long
    // If currently messaging isnt in friends list
    // AND You dont have friend request from them
    // AND they dont have friend request from you
    if (   !client->getUser().getFriendsList().contains(currentlyMessaging.get_username())
        && !client->getUser().getFriendRequestList().contains(currentlyMessaging.get_username())
        && !currentlyMessaging.getFriendRequestList().contains(client->getUser().get_username()))
    {
        qDebug() << "Sending friend request.";
        client->friendRequest(currentlyMessaging);
    }
}


//Not used, will be used later for servers!
void PrivateMessages::loadPage()
{
    //deserialize?
    QFile jsonFile("test.json");

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << "test.json";
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (!doc.isArray())
    {
        qDebug() << "JSON format is incorrect!";
        return;
    }



    QJsonArray jsonArray = doc.array();

    for (const QJsonValue &value : jsonArray)
    {
        if (!value.isObject()) continue;

        QJsonObject obj = value.toObject();
        QString to = obj["To"].toString();
        QString from = obj["From"].toString();
        QString msg = obj["Message"].toString();


        //Gotta figure this out
        //Have to make new User objects for 'from' and 'to'.
        //Or have to use existing objects but idk how to "find" them
        //Message message(from, to, msg);

        qDebug() << "To:" << to << "| From:" << from <<"| Msg:" << msg;
    }
}

