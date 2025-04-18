#ifndef DMLISTMODEL_H
#define DMLISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include "User.h"
#include "message.h"
#include "oomwidget.h"

struct DMData {
    bool isGroup;
    QString groupName; //user or groupname
    QStringList members; //? dont think this is needed
    QString owner;
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

    void messageReceived(const QString& user, const Message msg);

    QList<Message> getMessageHistory(const QString &user) const;

    void addUserToDMList(const QString &user);
    void addGroupToDMList(const QString& owner, const QString& groupName, const QStringList& members);

    bool isGroup(DMData dm);

private:
    QList<QString> dmList;
    QHash<QString, DMData> dmMap;
    OOMWidget* oomWidget;
};

Q_DECLARE_METATYPE(QList<Message>)

#endif // DMLISTMODEL_H
