#include "profile.h"
#include "ui_profile.h"

Profile::Profile(QWidget *parent, QString *username)
    : QWidget(parent)
    , ui(new Ui::Profile)
    , username(username)
{
    ui->setupUi(this);
    ui->usernameLabel->setText(*username);
}

Profile::~Profile()
{
    delete ui;
}
