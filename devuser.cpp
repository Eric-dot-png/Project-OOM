#include "devuser.h"

DevUser::DevUser(QObject *parent)
    : BaseUser(parent)
{}

DevUser::DevUser(int id, const QString &username, QObject *parent)
    : BaseUser(id, username, parent)
{}

DevUser::~DevUser() {}
