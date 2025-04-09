#ifndef DMLISTMODEL_H
#define DMLISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include "User.h"
#include "message.h"
#include "oomwidget.h"

struct DMData {
    User user;
    QList<Message> messageHistory; // not used
};

class DMListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DMListModel(OOMWidget* widget, QObject *parent = nullptr)
        : QAbstractListModel(parent), oomWidget(widget) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    void messageReceived(const User& user, const Message msg);

    QList<Message> getMessageHistory(const User &user) const;

    void addUserToDMList(const User &user);

private:
    QList<User> dmList;
    QHash<User, DMData> dmMap;
    OOMWidget* oomWidget;
};

Q_DECLARE_METATYPE(QList<Message>)

#endif // DMLISTMODEL_H
