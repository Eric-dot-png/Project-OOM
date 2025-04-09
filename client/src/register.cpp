// Name: Alex Franke
// File: register.cpp
// OOM project
//
// Description: This file defines the Register class which handles the registration process in a
//              Discord-like Qt application. It validates the user's inputs (username, email, and
//              passwords), and interacts with the Client class to send a create-account request.

#include "register.h"
#include "User.h"
#include "Client.h"
#include <QDebug>
#include <iostream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMovie>

// Constructor: Initializes the Register UI, hides the loading icon by default,
// and sets up signals/slots for the form's buttons and fields.
Register::Register(QWidget *parent)
    : OOMWidget(parent), registerUi(new Ui::Register), loadingIcon(new QMovie(":/images/images/OOMloading.gif"))
{
    registerUi->setupUi(this);                 // Sets up the user interface from the .ui file
    registerUi->testLabel->clear();            // Clears any placeholder text
    registerUi->loadingLabel->setVisible(false); // Hides the loading animation initially

    // Go back to the login screen when the user clicks 'Back'
    connect(registerUi->backToLoginButton, &QPushButton::clicked, this, &Register::handleBack);

    // Attempt to create an account when the user clicks 'Create Account'
    connect(registerUi->createAccountButton, &QPushButton::clicked, this, &Register::handleRegister);

    // Show or hide the password text when 'Show Password' is clicked
    connect(registerUi->showPasswordButton, &QPushButton::clicked, this, &Register::showPassword);

    // When account creation fails (emitted by the Client), enable the 'Back to Login' button again
    connect(client, &Client::accountNotCreated, this, &Register::setBackEnabled);

    connect(client, &Client::accountCreated, this, [=](){
        resetForm(true);
        registerUi->loadingLabel->setVisible(false);
        registerUi->backToLoginButton->setEnabled(true);
    });
}

// Destructor: Cleans up the dynamically allocated UI.
Register::~Register()
{
    delete registerUi;
}

/* ----------------- Helpers ----------------- */

// Validates the password to ensure it contains at least 8 characters, 1 uppercase, 1 lowercase,
// 1 digit, and 1 special character.
bool Register::isValidPassword(const QString &password)
{
    // This regex checks the required password constraints
    QRegularExpression re("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[^A-Za-z0-9]).{8,}$");
    QRegularExpressionMatch match = re.match(password);
    return match.hasMatch();
}

// Validates that the email provided has a valid format and belongs to one of the specified domains.
bool Register::isValidEmail(const QString &email)
{
    // Checks if the string matches gmail.com, yahoo.com, or cougars.ccis.edu
    QRegularExpression re("^[a-zA-Z0-9._%+-]+@(gmail\\.com|yahoo\\.com|cougars\\.ccis\\.edu)$");
    QRegularExpressionMatch match = re.match(email);
    return match.hasMatch();
}

// Resets form fields. If 'fullForm' is true, clears all input fields (username, password, etc.),
// and always clears validation labels.
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

/* ----------------- Slots ----------------- */

// Triggered when 'Create Account' is clicked. Checks input validity, displays a loading icon,
// and sends a create-account request to the Client if everything is valid.
void Register::handleRegister()
{
    registerUi->loadingLabel->setVisible(true);

    // Set the loading animation
    registerUi->loadingLabel->setMovie(loadingIcon);
    loadingIcon->start();

    // Clear any leftover error messages, but keep user input
    resetForm(false);

    // Retrieve data from the form
    QString u = registerUi->usrnameEdit->text();
    QString p = registerUi->passwordEdit->text();
    QString p2 = registerUi->confirmPasswordEdit->text();
    QString e = registerUi->emailEdit->text();

    // Prepare strings to hold potential error messages
    QString emailError;
    QString passwordError;

    // Validate email format
    if (!isValidEmail(e))
        emailError = "Invalid email. ";

    // Check for matching passwords and valid password requirements
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

    // If any error message is set, display them and stop the loading icon.
    if (!emailError.isEmpty() || !passwordError.isEmpty())
    {
        registerUi->emailConfirmLabel->setText(emailError);
        registerUi->passwordMatchLabel->setText(passwordError);

        loadingIcon->stop();
        registerUi->loadingLabel->setVisible(false);
    }
    else
    {
        // Disable 'Back to Login' button during the account creation process
        registerUi->backToLoginButton->setEnabled(false);

        // Create a User object and send a create account request via the Client
        User user(u, p, e);
        client->createAccount(user);
    }
}

// Triggered when 'Back to Login' is clicked. Resets the form and emits the backToLogin() signal.
void Register::handleBack()
{
    qDebug() << "Going back to Login Page...";
    resetForm(true);
    emit backToLogin();
}

// Toggles the visibility of the password field. When toggled, it shows/hides both
// 'passwordEdit' and 'confirmPasswordEdit' fields in plain text or password mask mode.
void Register::showPassword()
{
    if (registerUi->passwordEdit->echoMode() == QLineEdit::Normal)
    {
        registerUi->passwordEdit->setEchoMode(QLineEdit::Password);
        registerUi->confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    }
    else
    {
        registerUi->passwordEdit->setEchoMode(QLineEdit::Normal);
        registerUi->confirmPasswordEdit->setEchoMode(QLineEdit::Normal);
    }
}

// Enables or disables the 'Back to Login' button depending on its current state.
// Used when the Client signals that account creation failed, allowing the user
// to navigate back again.
void Register::setBackEnabled()
{
    bool e = registerUi->backToLoginButton->isEnabled();
    if (e)
        qDebug() << "True";
    else
        qDebug() << "False";

    registerUi->backToLoginButton->setEnabled(!e);
}
