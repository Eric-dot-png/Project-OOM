#include "message.h"

Message::Message(const QString &sender, const QString &receiver, const QString &msg, QObject *parent)
    : sender(sender), receiver(receiver), msg(msg), QObject(parent)
{

}

