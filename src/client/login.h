//Name: Alex Franke
//File: login.h
//OOM project


#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "ui_login.h"
#include "Client.h"

class Login : public QWidget {
    Q_OBJECT

public:
    // ADD CLIENT TO ALL WIDGETS
    explicit Login(oom::Client *client, QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccess();
    void registerRequested();

private slots:
    void handleLogin();
    void goToRegister();

private:
    oom::Client *client;
    Ui::Login *loginUi;
};

#endif // LOGIN_H
