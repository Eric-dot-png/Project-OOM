//Name: Alex Franke
//File: login.h
//OOM project

/*
 * Login form.
 * Allows the user to login if they already have an account,
 * or will allow the user to press a button to register for an account.
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "oomwidget.h"
#include "ui_login.h"
#include "Client.h"

class Login : public OOMWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccess();
    void registerRequested();

private slots:
    void handleLogin();
    void goToRegister();

private:
    Ui::Login *loginUi;
};

#endif // LOGIN_H
