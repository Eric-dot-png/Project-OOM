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

private:
    Ui::Register *registerUi;
    oom::Client *client;
};

#endif // REGISTER_H
