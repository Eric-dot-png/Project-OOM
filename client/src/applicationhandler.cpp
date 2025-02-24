//Name: Alex Franke
//File: applicationhander.cpp
//OOM project

#include "applicationhandler.h"
#include <QVBoxLayout>

/* TODO:
 * Make servers a blank template page.
 * Server page will be populated based on json file
*/

ApplicationHandler::ApplicationHandler(QWidget *parent)
    : OOMWidget(parent)
{
    int port = 1234; // replace LocalHost and Port when ready
    client->connectToServer(QHostAddress::LocalHost, port);
    
    /*
     * Stacked widget to allow seemless transitioning between forms.
     * Stacked widget allows having a single window compared to
     * opening a new window and closing the old.
     */
    stackedWidget = new QStackedWidget;

    LoginWidget = new Login(this);
    RegisterWidget = new Register(this);
    PrivateMessagesWidget = new PrivateMessages(this);
    AuthenticationWidget = new authenticationCode(this);


    stackedWidget->addWidget(LoginWidget);
    stackedWidget->addWidget(RegisterWidget);
    stackedWidget->addWidget(PrivateMessagesWidget);
    stackedWidget->addWidget(AuthenticationWidget);


    stackedWidget->setCurrentIndex(0); // login page

    //The layout of the window
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    /* signal/slot connection explanation
     *
     * connect(
     *         object sending a signal,
     *         signal from object,
     *         class that holds the slot,
     *         slot(function) to be called when a signal is emitted
     *         )


     * Each connect function is a transition to a new form
     */

    //Goto Login form -> PrivateMessages form
    connect(LoginWidget, &Login::loginSuccess, this, [=]() {
        qDebug() << "Logged in, going to private messages form...";
        switchToWidget(stackedWidget->indexOf(PrivateMessagesWidget));
    });

    //Goto Login form -> Register form
    connect(LoginWidget, &Login::registerRequested, this, [=]() {
        qDebug() << "Going to register form...";
        switchToWidget(stackedWidget->indexOf(RegisterWidget));
    });

    //Goto Register form -> Login form using BACK button
    connect(RegisterWidget, &Register::backToLogin, this, [=]() {
        qDebug() << "Going back to login form...";
        switchToWidget(stackedWidget->indexOf(LoginWidget));
    });

    //Connect client to server
    connect(client, &Client::connectedToServer, this, [=]() {
        qDebug() << "clientside connectedToServer";
    });

    //Once account is created, move to authentication code form
    connect(client, &Client::accountCreated, this, [=]() {
        qDebug() << "clientside accountCreated";
        switchToWidget(stackedWidget->indexOf(AuthenticationWidget));
    });

    //After authentication code is validated, move to privateMessages
    connect(client, &Client::accountAuthenticated, this, [=]() {
        qDebug() << "Clientside account authenticated, moving to privateMessages.";
        switchToWidget(stackedWidget->indexOf(PrivateMessagesWidget));
    });

    //Also Goto Login form -> PrivateMessages
    connect(client, &Client::loginSuccess, this, [=]() {
        qDebug() << "clientside loggedin";
        switchToWidget(stackedWidget->indexOf(PrivateMessagesWidget));
    });
} //End of Constructor

void ApplicationHandler::switchToWidget(int index){
    if (index >= 0 && index < stackedWidget->count())
    {
        stackedWidget->setCurrentIndex(index);
    }
}
