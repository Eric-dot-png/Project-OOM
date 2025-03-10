#include "dmlistmodel.h"

DMListModel::DMListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void DMListModel::addUserToDMList(const User &user)
{
    if (dmList.contains(user)) return;

    beginInsertRows(QModelIndex(), 0, 0);
    dmList.prepend(user);
    dmMap[user] = {user, {}};
    endInsertRows();

    emit dataChanged(index(0), index(rowCount() - 1));
}

QList<Message> DMListModel::getMessageHistory(const User &user) const
{
    return dmMap.contains(user) ? dmMap[user].messageHistory : QList<Message>();
}

void DMListModel::messageReceived(const User& user, const Message msg)
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

    dmMap[user].messageHistory.append(msg);
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
    const User &user = dmList.at(index.row());

    if (!dmMap.contains(user))
    {
        //qDebug() << "User not found in dmMap: " << user.get_username();
        return QVariant();
    }

    const DMData& dm = dmMap[user];
    //qDebug() << "Retrieved DMData for: " << dm.user.get_username();

    if (role == Qt::DisplayRole) {
        QString msg = dm.messageHistory.isEmpty() ? "(No messages)" : dm.messageHistory.last().get_msg();
        //qDebug() << "Data: " << msg;
        return QString("%1: %2").arg(dm.user.get_username(), msg);
    } else if (role == Qt::UserRole) {
        //qDebug() << "Returning user object";
        return QVariant::fromValue(dm.user);
    } else if (role == Qt::UserRole + 1) {
        //qDebug() << "Returning username";
        return dm.user.get_username();
    } else if (role == Qt::UserRole + 2) {
        //qDebug() << "Returning messageHistory";
        return QVariant::fromValue(dm.messageHistory);
    }

    //qDebug() << "unhandled role in data()";
    return QVariant();
}
