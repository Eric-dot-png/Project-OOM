#ifndef MESSAGE_H
#define MESSAGE_H

#include "User.h"
#include <QMetaType>

class Message
{
public:
    //Later change this to User, User, Qstring i think?
    explicit Message(const QString &sender, const QString &receiver, const QString &msg);
    
    QString get_sender() const { return sender.get_username(); }
    QString get_receiver() const { return receiver.get_username(); }
    QString get_msg() const { return msg; }


    //For QVariant
    Message() = default;
    Message(const Message &other) = default;
    bool operator==(const Message &o) const
    {
        return (sender == o.sender) && (receiver == o.receiver) && (msg == o.msg);
    }
private:
    User sender;
    User receiver;
    QString msg;
};

inline uint qHash(const Message &key, uint seed = 0)
{
    return qHash(key.get_sender(), seed);
}

Q_DECLARE_METATYPE(Message)

#endif // MESSAGE_H
