#include "message.h"

Message::Message(User sender, User receiver, QString msg, QObject *parent)
    : sender(sender), receiver(receiver), msg(msg), QObject(parent)
{

}
