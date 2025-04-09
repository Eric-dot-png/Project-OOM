#include "OOMTextBrowser.h"

OOMTextBrowser::OOMTextBrowser(QWidget *parent) : QTextBrowser(parent) {}

void OOMTextBrowser::appendMessage(const Message &msg, bool history)
{
    append(QString("[%1]: %2").arg(msg.get_sender(), msg.get_msg()));
}
