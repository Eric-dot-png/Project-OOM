#include "user.h"

User::User(QObject *parent)
    : BaseUser(parent)
{}

User::User(int id, const QString &username, QObject *parent)
    : BaseUser(id, username, parent) {}

User::~User() {}
