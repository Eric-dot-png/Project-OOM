//Name: Alex Franke
//File: authenticationcode.h
//OOM project

/*
 * Form to accept an authentication code/token to
 * confirm regristration.
 */

#ifndef AUTHENTICATIONCODE_H
#define AUTHENTICATIONCODE_H

#include <QWidget>
#include "Client.h"
#include "ui_authenticationcode.h"

class authenticationCode : public QWidget
{
    Q_OBJECT

public:
    explicit authenticationCode(oom::Client *client, QWidget *parent = nullptr);
    ~authenticationCode();

private slots:
    void authenticateUser();
    void authenticationFailed();

private:
    oom::Client *client;
    Ui::authenticationCode *AuthUi;
};

#endif // AUTHENTICATIONCODE_H
