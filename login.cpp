#include "login.h"
#include "ui_login.h"
#include "profile.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
    , authenticated(false)
{
    ui->setupUi(this);
    QWidget::setTabOrder(ui->usernameTextbox, ui->passwordTextbox);
    QWidget::setTabOrder(ui->passwordTextbox, ui->loginButton);
    QWidget::setTabOrder(ui->loginButton, ui->registerButton);
    QWidget::setTabOrder(ui->registerButton, ui->usernameTextbox);
}

Login::~Login()
{
    delete ui;
}

void Login::on_registerButton_clicked()
{
    // In the future, this will send data to server/database to register an account.
    //ui->registerNotImplementedLabel->setEnabled(true);
    ui->registerNotImplementedLabel->setText("Not Implemented yet!");
}


void Login::on_loginButton_clicked()
{
    QString *username = new QString(ui->usernameTextbox->text());
    QString *password = new QString(ui->passwordTextbox->text());

    bool usernameEmpty = false;
    bool passwordEmpty = false;

    ui->passwordTextbox->text() == "" ? passwordEmpty = true : passwordEmpty = false;
    ui->usernameTextbox->text() == "" ? usernameEmpty = true : usernameEmpty = false;

    ui->passwordTextbox->clear();

    //TODO: username/password authentication
    authenticated = true;

    if (authenticated && !usernameEmpty && !passwordEmpty) {
        //             nullptr because we don't want the login screen to be the parent ui
        //             the next screen might become parent of everything else TBD :)
        Profile *p = new Profile(nullptr, username);
        p->show();
        this->close();
    } else {
        ui->passwordTextbox->clear();
        ui->authFailedLabel->setText("Authentication failed! Username or password incorrect!");
    }


}

