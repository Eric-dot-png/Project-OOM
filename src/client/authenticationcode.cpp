#include "authenticationcode.h"
#include "ui_authenticationcode.h"

authenticationCode::authenticationCode(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent), AuthUi(new Ui::authenticationCode)
{
    AuthUi->setupUi(this);
    connect(AuthUi->submitButton, &QPushButton::clicked, this, &authenticationCode::authenticateUser);

}

authenticationCode::~authenticationCode()
{
    delete AuthUi;
}

void authenticationCode::authenticateUser()
{
    QString code = AuthUi->codeTextbox->text();
    qDebug() << code;
    //client->submitAuthCode(code);
}
