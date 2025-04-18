#include "dmlistmodel.h"


void DMListModel::addUserToDMList(const QString &user)
{
    if (dmList.contains(user)) return;

    beginInsertRows(QModelIndex(), 0, 0);
    dmList.prepend(user);
    dmMap[user] = {false, user};
    endInsertRows();

    emit dataChanged(index(0), index(rowCount() - 1));
}

void DMListModel::addGroupToDMList(const QString& owner, const QString& groupName, const QStringList& members)
{
    if (dmList.contains(groupName)) return;

    beginInsertRows(QModelIndex(), 0, 0);
    dmList.prepend(groupName);
    dmMap[groupName] = {true, groupName, members, owner};
    endInsertRows();

    emit dataChanged(index(0), index(rowCount() - 1));
}

QList<Message> DMListModel::getMessageHistory(const QString &user) const
{
    return dmMap.contains(user) ? oomWidget->getClient()->getDMsWith(user)->allMessages() : QList<Message>();
    //return dmMap.contains(user) ? dmMap[user].messageHistory : QList<Message>();
}

void DMListModel::messageReceived(const QString& user, const Message msg)
{
    if (!dmMap.contains(user))
        addUserToDMList(user);

    int oldIndex = dmList.indexOf(user);

    if (oldIndex > 0)
    {
        beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), 0);
        dmList.move(oldIndex, 0);
        endMoveRows();
    } else if (oldIndex == -1)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        dmList.prepend(user);
        endInsertRows();
    }

    //dmMap[user].messageHistory.append(msg);
    emit dataChanged(index(0), index(0));
}

int DMListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    //qDebug() << "rowcount() called, current size: " << dmList.size();
    return dmList.size();
}

QVariant DMListModel::data(const QModelIndex& index, int role) const
{
    //qDebug() << "Inside data()";
    if (dmList.isEmpty())
    {
        //qDebug() << "DMListModel::data() called but dmList is EMPTY!";
        return QVariant();
    }

    if (!index.isValid() || index.row() >= dmList.size())
    {
        //qDebug() << "Invalid index accessed in DMListModel::data()";
        return QVariant();
    }

    //qDebug() << "dmList size: " << dmList.size() << ", Accessing row:" << index.row();
    const QString &user = dmList.at(index.row());

    if (!dmMap.contains(user))
    {
        //qDebug() << "User not found in dmMap: " << user.get_username();
        return QVariant();
    }

    const DMData& dm = dmMap[user];
    //qDebug() << "Retrieved DMData for: " << dm.groupName;

    if (role == Qt::DisplayRole) {
        //qDebug() << "returning DisplayRole?";
        return QString("%1").arg(dm.groupName);
    } else if (role == Qt::UserRole) {
        //qDebug() << "Returning user object";
        return QVariant::fromValue(dm.groupName);
    } else if (role == Qt::UserRole + 1) {
        //qDebug() << "Returning username: " << dm.groupName;
        return dm.groupName;
    } else if (role == Qt::UserRole + 2) {
        //qDebug() << "Returning messageHistory";
        try{
            return QVariant::fromValue(oomWidget->getClient()->getDMsWith(user)->allMessages());
        } catch (const std::exception &e) {
            //qDebug() << "Exception in getDMsWith:" << e.what();
            return QVariant();
        }
    } else if (role == Qt::UserRole + 3) {
        return dm.isGroup;
    } else if (role == Qt::UserRole + 4)
        return dm.owner;

    //qDebug() << "unhandled role in data()";
    return QVariant();
}
