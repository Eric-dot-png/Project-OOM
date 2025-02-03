#include "applicationhandler.h"
#include <QVBoxLayout>

/*
 * Make servers a blank template page.
 * Server page will be populated based on json file
 *
*/

ApplicationHandler::ApplicationHandler(oom::Client *client, QWidget *parent)
    : client(client), QWidget(parent)
{
    int port = 1234; // replace LocalHost and Port when ready
    client->connectToServer(QHostAddress::LocalHost, port);
    
    stackedWidget = new QStackedWidget;

    LoginWidget = new Login(client, this);
    RegisterWidget = new Register(client, this);

    stackedWidget->addWidget(LoginWidget);
    stackedWidget->addWidget(RegisterWidget);

    stackedWidget->setCurrentIndex(0); // login page

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    connect(LoginWidget, &Login::loginSuccess, this, [=]() {
        switchToWidget(2);
    });

    connect(LoginWidget, &Login::registerRequested, this, [=]() {
        switchToWidget(1);
    });

    connect(RegisterWidget, &Register::backToLogin, this, [=]() {
        switchToWidget(0);
    });
    
}

void ApplicationHandler::switchToWidget(int index){
    if (index >= 0 && index < stackedWidget->count())
    {
        stackedWidget->setCurrentIndex(index);
    }
}
