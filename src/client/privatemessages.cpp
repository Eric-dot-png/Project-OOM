//Name: Alex Franke
//File: privateMessages.cpp
//OOM Project

#include "privatemessages.h"
#include "ui_privatemessages.h"

PrivateMessages::PrivateMessages(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), ui(new Ui::PrivateMessages)
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

void PrivateMessages::onEnterKeyPressed()
{
    qDebug() << "Sending message: " << ui->textEdit->toPlainText();
    ui->textBrowser->append(ui->textEdit->toMarkdown());
    ui->textEdit->clear();
}
