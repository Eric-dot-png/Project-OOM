//Name: Alex Franke
//File: login.cpp
//OOM project


#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QMovie>
#include <iostream>

Login::Login(QWidget *parent)
    : OOMWidget(parent), loginUi(new Ui::Login)

{
    loginUi->setupUi(this);
    loginUi->loadingLabel->setVisible(false);
    //Connect buttons to the below slots
    connect(loginUi->loginButton, &QPushButton::clicked, this, &Login::handleLogin);
    connect(loginUi->registerButton, &QPushButton::clicked, this, &Login::goToRegister);

    connect(client, &Client::loginFail, this, [=]() {
        loginUi->loadingLabel->setVisible(false);
        loginUi->authFailedLabel->setText("Login failed! \nPlease try again or make an account if you don't have one!");
    });

    connect(client, &Client::loginSuccess, this, [=]() {
        loginUi->loadingLabel->setVisible(false);
        loginUi->authFailedLabel->setVisible(false);
    });

    loginUi->styleCheckbox->setChecked(false);

    connect(loginUi->styleCheckbox, &QCheckBox::toggled, this, [=](bool checked) {
        if (checked)
            qApp->setStyleSheet(loadStyleSheet(styles["DarkMode"]));
        else
            qApp->setStyleSheet(loadStyleSheet(styles["LightMode"]));
    });
}

Login::~Login() {
    delete loginUi;
}

//When login button is clicked, this slot is fired.
//Gets username and password, sends to client to login.
void Login::handleLogin() {
    loginUi->loadingLabel->setVisible(true);
    //Loading Icon
    QMovie *loadingIcon = new QMovie(":/images/images/OOMloading.gif");
    loginUi->loadingLabel->setMovie(loadingIcon);
    loadingIcon->start();

    QString usr = loginUi->usernameTextbox->text();
    QString pwd = loginUi->passwordTextbox->text();
    
    User u(usr,pwd);
    client->login(u);

}

//When register button is clicked, this will fire the
//registerRequested() SIGNAL to transition pages.
//The slot that receives this signal is in applicationHandler.cpp
void Login::goToRegister() {
    qDebug() << "Navigating to Register Page... ";
    loginUi->authFailedLabel->setVisible(false);
    loginUi->loadingLabel->setVisible(false);
    emit registerRequested();
}
