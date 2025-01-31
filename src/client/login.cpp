//Name: Alex Franke
//File: login.cpp
//OOM project


#include "login.h"
#include <QDebug>
#include <QVBoxLayout>

Login::Login(QWidget *parent) : QWidget(parent) {
    qDebug() << "Initializing login...";


    loginStackedWidget = new QStackedWidget(this);


    loginWidget = new QWidget(this);
    loginUi.setupUi(loginWidget);


    registerWidget = new QWidget(this);
    registerUi.setupUi(registerWidget);

    loginStackedWidget->addWidget(loginWidget);
    loginStackedWidget->addWidget(registerWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(loginStackedWidget);
    setLayout(mainLayout);

    qDebug() << "StackedWidget set up successfully";


    QPushButton *registerButton = loginWidget->findChild<QPushButton*>("registerButton");
    QPushButton *loginButton = loginWidget->findChild<QPushButton*>("loginButton");


    QPushButton *createAccountButton = registerWidget->findChild<QPushButton*>("createAccountButton");
    QPushButton *backToLoginButton = registerWidget->findChild<QPushButton*>("backToLoginButton");


    if (registerButton) {
        connect(registerButton, &QPushButton::clicked, this, &Login::on_registerButton_clicked);
    } else {
        qDebug() << "Error: registerButton not found!";
    }

    if (loginButton) {
        connect(loginButton, &QPushButton::clicked, this, &Login::on_loginButton_clicked);
    } else {
        qDebug() << "Error: loginButton not found!";
    }

    if (createAccountButton) {
        connect(createAccountButton, &QPushButton::clicked, this, &Login::on_createAccountButton_clicked);
    } else {
        qDebug() << "Error: createAccountButton not found!";
    }

    if (backToLoginButton) {
        connect(backToLoginButton, &QPushButton::clicked, this, &Login::on_backToLoginButton_clicked);
    } else {
        qDebug() << "Error: backToLoginButton not found!";
    }


    //Login form tab order
    QWidget::setTabOrder(loginUi.usernameTextbox, loginUi.passwordTextbox);
    QWidget::setTabOrder(loginUi.passwordTextbox, loginUi.loginButton);
    QWidget::setTabOrder(loginUi.loginButton, loginUi.registerButton);
    QWidget::setTabOrder(loginUi.registerButton, loginUi.usernameTextbox);

    //Register form tab order
    //usr->pwd->email->regButton
    QWidget::setTabOrder(registerUi.usrnameEdit, registerUi.passwordEdit);
    QWidget::setTabOrder(registerUi.passwordEdit, registerUi.emailEdit);
    QWidget::setTabOrder(registerUi.emailEdit, registerUi.createAccountButton);
    QWidget::setTabOrder(registerUi.createAccountButton, registerUi.backToLoginButton);
    QWidget::setTabOrder(registerUi.backToLoginButton, registerUi.usrnameEdit);
}

Login::~Login() {

    /*

    Anything that is a child of QT of any sort, do not delete it
    You can see below that I had these which are all children
    of Qt in some way and it causes the program to crash when
    exiting because Qt automatically deletes everything you make

    If you uncomment the 3 below statements the run/close the program
    it will segmentation fault

    */

    // delete loginStackedWidget;
    // delete loginWidget;
    // delete registerWidget;
}

void Login::on_registerButton_clicked() {
    qDebug() << "Switching to register.ui ...";
    loginStackedWidget->setCurrentWidget(registerWidget);
}

void Login::on_backToLoginButton_clicked() {
    qDebug() << "Switching to login.ui ...";
    loginStackedWidget->setCurrentWidget(loginWidget);
}

void Login::on_loginButton_clicked() {
    qDebug() << "Attempting login...";

    bool auth = true;
    if (auth) {
        emit loginSuccessful();
    }
}

void Login::on_createAccountButton_clicked() {
    qDebug() << "Creating account...";

    bool regSuccess = true;
    if (regSuccess) {
        loginStackedWidget->setCurrentWidget(loginWidget);
        qDebug() << "Account created. Returning to login.";
    }
}
