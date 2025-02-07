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
    PrivateMessagesWidget = new PrivateMessages(client, this);
    AuthenticationWidget = new authenticationCode(client, this);

                                                    //index
    stackedWidget->addWidget(LoginWidget);           // 0
    stackedWidget->addWidget(RegisterWidget);        // 1
    stackedWidget->addWidget(PrivateMessagesWidget); // 2
    stackedWidget->addWidget(AuthenticationWidget);  // 3

    stackedWidget->setCurrentIndex(0); // login page

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    //Goto Login form -> PrivateMessages form
    connect(LoginWidget, &Login::loginSuccess, this, [=]() {
        switchToWidget(2);
    });

    //Goto Login form -> Register form
    connect(LoginWidget, &Login::registerRequested, this, [=]() {
        switchToWidget(1);
    });

    //Goto Register form -> Login form using BACK button
    connect(RegisterWidget, &Register::backToLogin, this, [=]() {
        switchToWidget(0);
    });

    //Connect client to server
    connect(client, &oom::Client::connectedToServer, this, [=]() {
        qDebug() << "clientside connectedToServer";
    });


    connect(client, &oom::Client::accountCreated, this, [=]() {
        qDebug() << "clientside accountCreated";
        switchToWidget(3);
    });

    //Also Goto Login form -> PrivateMessages
    connect(client, &oom::Client::loginSuccess, this, [=]() {
        switchToWidget(2);
        qDebug() << "clientside loggedin";
        
    });
}

void ApplicationHandler::switchToWidget(int index){
    if (index >= 0 && index < stackedWidget->count())
    {
        stackedWidget->setCurrentIndex(index);
    }
}
