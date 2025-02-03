#include "register.h"
#include <QDebug>

Register::Register(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), registerUi(new Ui::Register)

{
    registerUi->setupUi(this);

    connect(registerUi->backToLoginButton, &QPushButton::clicked, this, &Register::handleBack);
}

Register::~Register() {
    delete registerUi;
}

void Register::handleBack() {
    qDebug() << "Going back to Login Page...";
    emit backToLogin();
}
