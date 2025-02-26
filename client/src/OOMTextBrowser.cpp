#include "OOMTextBrowser.h"

OOMTextBrowser::OOMTextBrowser(QWidget *parent) : QTextBrowser(parent) {}

void OOMTextBrowser::appendMessage(Message msg)
{
    //store msg in dictionary with sender/receiver

    QMap<QString, QString> msgData;
    msgData["Sender"] = msg.get_sender();
    msgData["Receiver"] = msg.get_receiver();
    msgData["Msg"] = msg.get_msg();
    messageList.append(msgData);
    qDebug() << msg.get_sender() + ' ' + msg.get_receiver() + ' ' + msg.get_msg();

    append(QString("[%1 -> %2]: %3").arg(msg.get_sender(), msg.get_receiver(), msg.get_msg()));
}
