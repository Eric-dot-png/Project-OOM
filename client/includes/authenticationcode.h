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
#include "oomwidget.h"
#include "Client.h"
#include "ui_authenticationcode.h"

class authenticationCode : public OOMWidget
{
    Q_OBJECT

public:
    explicit authenticationCode(QWidget *parent = nullptr);
    ~authenticationCode();

private slots:
    void authenticateUser();
    void authenticationFailed();

private:
    Ui::authenticationCode *AuthUi;
};

#endif // AUTHENTICATIONCODE_H
