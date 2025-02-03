//Name: Alex Franke
//File: applicationhander.h
//OOM project

#ifndef APPLICATIONHANDLER_H
#define APPLICATIONHANDLER_H

#include <QWidget>
#include <QStackedWidget>
#include "login.h"
#include "register.h"

#include "Client.h"

// Class to handle switching between widgets (a.k.a. different pages).

class ApplicationHandler : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationHandler(oom::Client *client, QWidget *parent = nullptr);
    void switchToWidget(int index);

private:
    oom::Client *client;
    QStackedWidget *stackedWidget;
    Login *LoginWidget;
    Register *RegisterWidget;
};

#endif // APPLICATIONHANDLER_H
