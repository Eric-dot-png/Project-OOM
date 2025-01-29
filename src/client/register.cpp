#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    QWidget::setTabOrder(ui->usrnameEdit, ui->passwordEdit);
    QWidget::setTabOrder(ui->passwordEdit, ui->emailEdit);
    QWidget::setTabOrder(ui->emailEdit, ui->registerButton);
}

Register::~Register()
{
    delete ui;
}

void Register::on_backButton_clicked()
{
    emit showLogin();
    this->close();
}

void Register::on_registerButton_clicked()
{
    QString usrname = ui->usrnameEdit->text();
    QString password = ui->passwordEdit->text();
    QString email = ui->emailEdit->text();

    bool usernameEmpty = false;
    bool passwordEmpty = false;
    bool emailEmpty = false;

    ui->passwordEdit->text() == "" ? passwordEmpty = true : passwordEmpty = false;
    ui->usrnameEdit->text() == "" ? usernameEmpty = true : usernameEmpty = false;
    ui->emailEdit->text() == "" ? emailEmpty = true : emailEmpty = false;

    ui->usrnameEdit->clear();
    ui->passwordEdit->clear();
    ui->emailEdit->clear();


    //temp
    emit showLogin();
    this->close();
}


void Register::on_emailEdit_returnPressed()
{
    on_registerButton_clicked();
}


void Register::on_passwordEdit_returnPressed()
{
    on_registerButton_clicked();
}


void Register::on_usrnameEdit_returnPressed()
{
    on_registerButton_clicked();
}


void Register::on_registerButton_pressed(){}

