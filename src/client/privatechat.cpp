#include "privatechat.h"
#include "ui_privatechat.h"

PrivateChat::PrivateChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::on_sendMsgButton_clicked()
{
    QString msg = ui->clientTextbox->toPlainText();
    ui->chatBrowser->append(msg);
    ui->clientTextbox->clear();
}

