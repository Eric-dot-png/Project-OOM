//Name: Alex Franke
//File: login.h
//OOM project


#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QStackedWidget>
#include "ui_login.h"
#include "ui_register.h"

class Login : public QWidget {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccessful();

private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();
    void on_createAccountButton_clicked();
    void on_backToLoginButton_clicked();

private:
    Ui::Login loginUi;
    Ui::Register registerUi;
    QStackedWidget *loginStackedWidget;
    QWidget *loginWidget;
    QWidget *registerWidget;
};

#endif // LOGIN_H
