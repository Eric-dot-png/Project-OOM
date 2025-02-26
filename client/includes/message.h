#ifndef MESSAGE_H
#define MESSAGE_H

#include "User.h"

class Message
{
public:
    explicit Message(const QString &sender, const QString &receiver, const QString &msg);

    
    QString get_sender() const { return sender.get_username(); }
    QString get_receiver() const { return receiver.get_username(); }
    QString get_msg() const { return msg; }

signals:

private:
    User sender;
    User receiver;
    QString msg;
};

#endif // MESSAGE_H
