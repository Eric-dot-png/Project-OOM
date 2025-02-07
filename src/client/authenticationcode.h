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

private:
    oom::Client *client;
    Ui::authenticationCode *AuthUi;
};

#endif // AUTHENTICATIONCODE_H
