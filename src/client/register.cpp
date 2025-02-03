#include "register.h"
#include "User.h"
#include "Client.h"
#include <QDebug>
#include <iostream>

Register::Register(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), registerUi(new Ui::Register)

{
    registerUi->setupUi(this);

    connect(registerUi->backToLoginButton, &QPushButton::clicked, this, &Register::handleBack);
    connect(registerUi->createAccountButton, &QPushButton::clicked, this, &Register::handleRegister);
    //connect(client, oom::Client::connectToServer, this, &Register::usernameUnavailable)
}

Register::~Register() {
    delete registerUi;
}

void Register::handleRegister() {

    QString u = registerUi->usrnameEdit->text();
    QString p = registerUi->passwordEdit->text();
    QString e = registerUi->emailEdit->text();
    
    registerUi->testLabel->setText(u);

    User user(u, p, e);
    client->createAccount(user);

    
}

void Register::handleBack() {
    qDebug() << "Going back to Login Page...";
    emit backToLogin();
}
