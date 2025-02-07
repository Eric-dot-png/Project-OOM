//Name: Alex Franke
//File: register.h
//OOM project

/*
 * Register form.
 * This form allows for username, password and email input.
 * After registering, the form will transition to the
 * authenticationCode form.
 */

#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "Client.h"
#include "ui_register.h"

class Register : public QWidget
{
    Q_OBJECT
public:
    explicit Register(oom::Client *client, QWidget *parent = nullptr);
    ~Register();

signals:
    void backToLogin();

private slots:
    void handleBack();
    void handleRegister();
    void showPassword();

    //void usernameUnavailable();
    //void invalidEmail();

    //void usernameUnavailable();
    //void invalidEmail();

private:
    Ui::Register *registerUi;
    oom::Client *client;

    bool isValidEmail(const QString &);
    bool isValidPassword(const QString &);
    void resetForm(bool fullForm);
};

#endif // REGISTER_H
