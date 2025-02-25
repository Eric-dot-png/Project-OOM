//Name: Alex Franke
//File: applicationhander.h
//OOM project

/*
 * Wrapper class for managing widgets.
 *
 * This class creates an object of every widget the application
 * uses and allows for easy transitioning.
 *
 * Easily expandable through the connect() function.
 */

#ifndef APPLICATIONHANDLER_H
#define APPLICATIONHANDLER_H

#include <QTimer>
#include <QWidget>
#include <QStackedWidget>
#include "oomwidget.h"

//Widgets
#include "login.h"
#include "register.h"
#include "privatemessages.h"
#include "authenticationcode.h"

#include "Client.h"

// Class to handle switching between widgets (a.k.a. different pages).

class ApplicationHandler : public OOMWidget
{
    Q_OBJECT

public:
    explicit ApplicationHandler(QWidget *parent = nullptr);
    void switchToWidget(int index);
    
private:
    QStackedWidget *stackedWidget;
    Login *LoginWidget;
    Register *RegisterWidget;
    PrivateMessages *PrivateMessagesWidget;
    authenticationCode *AuthenticationWidget;
};

#endif // APPLICATIONHANDLER_H
