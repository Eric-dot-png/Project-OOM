//Name: Alex Franke
//File: privateMessages.cpp
//OOM Project

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "privatemessages.h"
#include "ui_privatemessages.h"
#include "User.h"

PrivateMessages::PrivateMessages(QWidget *parent)
    : OOMWidget(parent), ui(new Ui::PrivateMessages), currentlyMessaging(NULL)
{
    ui->setupUi(this);
    ui->friendNameLabel->clear();

    //This allows the textbox to detect the enter key
    enterFilter = new EnterKeyFilter(this);
    ui->textEdit->installEventFilter(enterFilter);

    connect(enterFilter, &EnterKeyFilter::enterPressed, this, &PrivateMessages::onEnterKeyPressed);
    connect(ui->searchUserTextbox, &QLineEdit::returnPressed, this, &PrivateMessages::searchUser);

    //connect(client, &Client::recievedDM, this, &PrivateMessages::recievedMessage);


    //If searching for a user fails
    connect(client, &Client::discoverUserFail, this, [=](const QString& username){
        ui->userNotFoundLabel->setText("User " + username +  " not found!");
        ui->friendNameLabel->clear();
    });

    //If searching for a user succeeds
    connect(client, &Client::discoverUserSucceed, this, [=](const QString& username){
        ui->userNotFoundLabel->clear();
        ui->friendNameLabel->setText("Now messaging: " + username);
        currentlyMessaging = User(username);
        qDebug() << currentlyMessaging.get_username();
    });
}

PrivateMessages::~PrivateMessages()
{
    delete ui;
}

//function to open a window showing all users?
//if so, need function to return list of all currently registered users.

void PrivateMessages::searchUser()
{
    qDebug() << "Searching for user: " << ui->searchUserTextbox->text();
    User u = User(ui->searchUserTextbox->text());

    if (u.get_username() == client->getUser().get_username()) return;

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
    qDebug() << "Sending message: " << ui->textEdit->toPlainText();

    QString fullMsg = formatClientMessage();
    QString msgContent = ui->textEdit->toPlainText();

    ui->textBrowser->append(formatClientMessage());
    ui->textEdit->clear();

    client->privateMessage(client->getUser(), msgContent);

    //Testing; will remove later
    //loadPage();
}

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
