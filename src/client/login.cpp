//Name: Alex Franke
//File: login.cpp
//OOM project


#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <iostream>

Login::Login(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), loginUi(new Ui::Login)

{
    loginUi->setupUi(this);
    connect(loginUi->loginButton, &QPushButton::clicked, this, &Login::handleLogin);
    connect(loginUi->registerButton, &QPushButton::clicked, this, &Login::goToRegister);
}

Login::~Login() {
    delete loginUi;
    delete client;
}

void Login::handleLogin() {

    //DELETE AFTER TESTING OR LOGIN WORKS!
    emit loginSuccess();
    //--------------------------------


    QString usr = loginUi->usernameTextbox->text();
    QString pwd = loginUi->passwordTextbox->text();
    
    User u(usr,pwd);
    client->login(u);
}

void Login::goToRegister() {
    qDebug() << "Navigating to Register Page... ";
    emit registerRequested();
}
