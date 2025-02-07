#include "register.h"
#include "User.h"
#include "Client.h"
#include <QDebug>
#include <iostream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

Register::Register(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), registerUi(new Ui::Register)

{
    registerUi->setupUi(this);
    registerUi->testLabel->clear();

    connect(registerUi->backToLoginButton, &QPushButton::clicked, this, &Register::handleBack);
    connect(registerUi->createAccountButton, &QPushButton::clicked, this, &Register::handleRegister);
<<<<<<< Updated upstream
    connect(registerUi->showPasswordButton, &QPushButton::clicked, this, &Register::showPassword);
    //connect(client, oom::Client::connectToServer, this, &Register::usernameUnavailable);
=======
    //connect(client, oom::Client::connectToServer, this, &Register::usernameUnavailable)
>>>>>>> Stashed changes
}

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
}

//To see if email input is actually an email address.
//Can add more email domains but i thought gmail and yahoo fine for now
bool Register::isValidEmail(const QString &email)
{
    //domain\\.com
    QRegularExpression re("^[a-zA-Z0-9._%+-]+@(gmail\\.com|yahoo\\.com)$");

    QRegularExpressionMatch match = re.match(email);

    return match.hasMatch();
}

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
}


/*
 * Slots
 */

void Register::handleRegister()
{
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
        passwordError = "Password must be at least 8 characters,\n"
                        "contain 1 uppercase letter, 1 lowercase letter,\n"
                        "1 digit, and 1 special character.";
    }

    if (!emailError.isEmpty() || !passwordError.isEmpty())
    {
        registerUi->emailConfirmLabel->setText(emailError);
        registerUi->passwordMatchLabel->setText(passwordError);
    }
    else
    {
        User user(u, p, e);
        client->createAccount(user);


        /*
         * if account is created then we
         * handleBack(); which will clear inputs and labels
         * then will send user back to login screen
         * else display error to user to try again
         */
    }
}

//Back to login
void Register::handleBack()
{
    qDebug() << "Going back to Login Page...";
    resetForm(true);
    emit backToLogin();
}



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
}
