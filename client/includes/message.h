#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include "User.h"

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(User sender, User receiver, QString msg, QObject *parent = nullptr);

    
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
