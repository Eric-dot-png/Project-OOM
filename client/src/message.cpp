#include "message.h"

Message::Message(const QString &sender, const QString &receiver, const QString &msg)
    : sender(sender), receiver(receiver), msg(msg)
{

}

