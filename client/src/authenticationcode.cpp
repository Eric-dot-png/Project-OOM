//Name: Alex Franke
//File: authenticationcode.cpp
//OOM project

#include "authenticationcode.h"
#include "ui_authenticationcode.h"

authenticationCode::authenticationCode(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), AuthUi(new Ui::authenticationCode)
{
    AuthUi->setupUi(this);
    connect(AuthUi->submitButton, &QPushButton::clicked, this, &authenticationCode::authenticateUser);
    connect(client, &oom::Client::accountAuthenticationFail, this, &authenticationCode::authenticationFailed);


}

authenticationCode::~authenticationCode()
{
    delete AuthUi;
}

/*
 * Receives the authentication code/token from the user
 * and sends it to the client which will either authenticate the user or not.
*/
void authenticationCode::authenticateUser()
{
    QString code = AuthUi->codeTextbox->text();
    qDebug() << code;
    client->submitAuthCode(code);
}

//If the user authentication fails, a label will appear telling them so.
void authenticationCode::authenticationFailed()
{
    AuthUi->authFailedLabel->setText("Authentication code incorrect!");
    AuthUi->codeTextbox->focusWidget();
}
