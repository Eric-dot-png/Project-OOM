#include "OOMTextBrowser.h"

OOMTextBrowser::OOMTextBrowser(QWidget *parent) : QTextBrowser(parent) {}

void OOMTextBrowser::appendMessage(const Message &msg, bool history)
{

    messageList.append(msg);
    //qDebug() << msg.get_sender() + ' ' + msg.get_receiver() + ' ' + msg.get_msg();

    append(QString("[%1 -> %2]: %3").arg(msg.get_sender(), msg.get_receiver(), msg.get_msg()));
}

void OOMTextBrowser::addMessageToHistory(const Message msg)
{
    messageList.append(msg);
}
