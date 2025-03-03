//Name: Alex Franke
//File: register.cpp
//OOM project

#include "register.h"
#include "User.h"
#include "Client.h"
#include <QDebug>
#include <iostream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMovie>

Register::Register(QWidget *parent)
    : OOMWidget(parent), registerUi(new Ui::Register)

{
    registerUi->setupUi(this);
    registerUi->testLabel->clear();


    connect(registerUi->backToLoginButton, &QPushButton::clicked, this, &Register::handleBack);
    connect(registerUi->createAccountButton, &QPushButton::clicked, this, &Register::handleRegister);
    connect(registerUi->showPasswordButton, &QPushButton::clicked, this, &Register::showPassword);
    //connect(client, Client::connectToServer, this, &Register::usernameUnavailable);


    //change this to signal from client when account creation fails
    connect(client, &Client::accountNotCreated, this, &Register::setBackEnabled);
} //end constructor

Register::~Register()
{
    delete registerUi;
}

/*
 * Helpers
 */


//8 characters, 1 number, 1 "special character", 1 uppercase, 1 lowercase
bool Register::isValidPassword(const QString &password)
{
    QRegularExpression re("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[^A-Za-z0-9]).{8,}$");

    QRegularExpressionMatch match = re.match(password);
    return match.hasMatch();
} //end Register::isValidPassword()

//To see if email input is actually an email address.
//Can add more email domains but i thought gmail, yahoo and cougarmail is fine for now
bool Register::isValidEmail(const QString &email)
{
    //domain\\.com
    QRegularExpression re("^[a-zA-Z0-9._%+-]+@(gmail\\.com|yahoo\\.com|cougars\\.ccis\\.edu)$");

    QRegularExpressionMatch match = re.match(email);

    return match.hasMatch();
} //end Register::isValidEmail()

//Reset labels, if returning to login form, clear all input boxes
void Register::resetForm(bool fullForm)
{
    registerUi->emailConfirmLabel->setText("");
    registerUi->passwordMatchLabel->setText("");

    if (fullForm)
    {
        registerUi->emailEdit->clear();
        registerUi->usrnameEdit->clear();
        registerUi->passwordEdit->clear();
        registerUi->confirmPasswordEdit->clear();
        registerUi->emailEdit->clear();
    }
} //end Register::resetForm()


/*
 * Slots
 */

void Register::handleRegister()
{
    QMovie *loadingIcon = new QMovie(":/images/images/OOMloading.gif");
    registerUi->loadingLabel->setMovie(loadingIcon);
    loadingIcon->start();

    resetForm(false);

    //Username
    QString u = registerUi->usrnameEdit->text();

    //p = password, p2 = confirm password
    QString p = registerUi->passwordEdit->text();
    QString p2 = registerUi->confirmPasswordEdit->text();

    //email
    QString e = registerUi->emailEdit->text();
    
    //Error messages for the labels
    QString emailError;
    QString passwordError;



    if (!isValidEmail(e))
        emailError = "Invalid email. ";

    if (p != p2)
    {
        passwordError = "Passwords do not match. ";
    }
    else if (!isValidPassword(p))
    {
        passwordError = "Password must be at least 8 characters, "
                        "contain 1 uppercase letter, 1 lowercase letter, "
                        "1 digit, and 1 special character.";
    }

    if (!emailError.isEmpty() || !passwordError.isEmpty())
    {
        registerUi->emailConfirmLabel->setText(emailError);
        registerUi->passwordMatchLabel->setText(passwordError);
        loadingIcon->stop();
    }
    else
    {
        registerUi->backToLoginButton->setEnabled(false);
        User user(u, p, e);
        client->createAccount(user);
        loadingIcon->stop();
        //after registration is accepted, the client will switch from
        //this form to another.
    }
} //end Register::handleRegister()

//Back to login
void Register::handleBack()
{
    qDebug() << "Going back to Login Page...";
    resetForm(true);

    emit backToLogin();
} //end Register::handleBack()



//Show/hide password button
void Register::showPassword()
{
    if (registerUi->passwordEdit->echoMode() == QLineEdit::Normal) {
        registerUi->passwordEdit->setEchoMode(QLineEdit::Password);
        registerUi->confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    } else {
        registerUi->passwordEdit->setEchoMode(QLineEdit::Normal);
        registerUi->confirmPasswordEdit->setEchoMode(QLineEdit::Normal);
    }
} //end Register::showPassword()

void Register::setBackEnabled()
{

    bool e = registerUi->backToLoginButton->isEnabled();
    if (e)
        qDebug() << "True";
    else
        qDebug() << "False";
    registerUi->backToLoginButton->setEnabled(!e);
} //end Register::setBackEnabled()
