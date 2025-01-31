//Name: Alex Franke
//File: baseuser.cpp
//OOM project


#include "baseuser.h"

BaseUser::BaseUser(QObject *parent)
    : QObject(parent),
    id(-1),
    username("Unknown")
{}

BaseUser::BaseUser(int id, const QString &username, QObject *parent)
    : QObject(parent),
    id(id),
    username(username)
{}

BaseUser::~BaseUser() {}

int BaseUser::getId() const {return id;}
QString BaseUser::getUsername() const {return username;}
