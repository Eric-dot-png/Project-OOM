#ifndef DMLISTMODEL_H
#define DMLISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include "User.h"
#include "message.h"

struct DMData {
    User user;
    QList<Message> messageHistory;
};

class DMListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DMListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    void messageReceived(const User& user, const Message msg);

    QList<Message> getMessageHistory(const User &user) const;

    void addUserToDMList(const User &user);

private:
    QList<User> dmList;
    QHash<User, DMData> dmMap;
};

Q_DECLARE_METATYPE(QList<Message>)

#endif // DMLISTMODEL_H
