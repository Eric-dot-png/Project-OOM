//Name: Alex Franke
//File: login.cpp
//OOM project


#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <iostream>

Login::Login(Client *client, QWidget *parent)
    : client(client), QWidget(parent), loginUi(new Ui::Login)

{
    loginUi->setupUi(this);

    //Connect buttons to the below slots
    connect(loginUi->loginButton, &QPushButton::clicked, this, &Login::handleLogin);
    connect(loginUi->registerButton, &QPushButton::clicked, this, &Login::goToRegister);
}

Login::~Login() {
    delete loginUi;
}

//When login button is clicked, this slot is fired.
//Gets username and password, sends to client to login.
void Login::handleLogin() {
    QString usr = loginUi->usernameTextbox->text();
    QString pwd = loginUi->passwordTextbox->text();
    
    User u(usr,pwd);
    client->login(u);



    //BADBADBADBAD just for testing.
    //emit loginSuccess();

}

//When register button is clicked, this will fire the
//registerRequested() SIGNAL to transition pages.
//The slot that receives this signal is in applicationHandler.cpp
void Login::goToRegister() {
    qDebug() << "Navigating to Register Page... ";
    emit registerRequested();
}
