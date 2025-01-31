#include "login.h"
#include "ui_login.h"
#include "profile.h"
#include "register.h"
#include <QTimer>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , currentStatus(Client::LoggedOut)
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

constexpr bool Login::getStatus(Client currentStatus, Client otherStatus) {
    return currentStatus == otherStatus;
}

void Login::update()
{
    QTimer::singleShot(2000, []() {
#ifdef QT_DEBUG
        qDebug() << "test";
#endif
    });
    if (getStatus(currentStatus, Client::LoggedOut))
    {
        ui->authFailedLabel->setText("UPDATING");
    }
}

void Login::on_registerButton_clicked()
{
    qDebug("registerButton clicked, redirecting...");
    Register * r = new Register();
    connect(r, &Register::showLogin, this, &Login::show);
    r->show();
    this->hide();
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

    update();

    if (authenticated && !usernameEmpty && !passwordEmpty) {
        //             nullptr because we don't want the login screen to be the parent ui
        //             the next screen might become parent of everything else TBD :)

        Profile *p = new Profile(nullptr, username);
        p->show();
        this->close();
    } else {
        update();
        ui->passwordTextbox->clear();
        //ui->authFailedLabel->setText("Authentication failed! Username or password incorrect!");
    }
}
