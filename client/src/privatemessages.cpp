//Name: Alex Franke
//File: privateMessages.cpp
//OOM Project

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "privatemessages.h"
#include "ui_privatemessages.h"

PrivateMessages::PrivateMessages(QWidget *parent)
    : OOMWidget(parent), ui(new Ui::PrivateMessages)
{
    ui->setupUi(this);

    //This allows the textbox to detect the enter key
    enterFilter = new EnterKeyFilter(this);
    ui->textEdit->installEventFilter(enterFilter);

    connect(enterFilter, &EnterKeyFilter::enterPressed, this, &PrivateMessages::onEnterKeyPressed);

}

PrivateMessages::~PrivateMessages()
{
    delete ui;
}

QString PrivateMessages::formatMessage()
{
    QString msg = ui->textEdit->toMarkdown();
    QString user = client->getUser().get_username();

    return "<b>" + user + "</b>:<br>" + msg;
}

void PrivateMessages::onEnterKeyPressed()
{
    qDebug() << "Sending message: " << ui->textEdit->toPlainText();

    //QString fullMsg = formatMessage();
    ui->textBrowser->append(formatMessage());
    ui->textEdit->clear();

    //Testing; will remove later
    loadPage();
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
