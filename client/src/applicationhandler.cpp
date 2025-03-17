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
    client->connectToServer(SERVER_IP, SERVER_PORT);
    
    /*
     * Stacked widget to allow seemless transitioning between forms.
     * Stacked widget allows having a single window compared to
     * opening a new window and closing the old.
     */
    stackedWidget = new QStackedWidget;


    //Keeping these to load at app start since they are more likely to be needed at app start
    LoginWidget = new Login(this);
    RegisterWidget = new Register(this);
    AuthenticationWidget = new authenticationCode(this);

    stackedWidget->addWidget(LoginWidget);
    stackedWidget->addWidget(RegisterWidget);
    stackedWidget->addWidget(AuthenticationWidget);

    // PrivateMessagesWidget = new PrivateMessages(this);
    // stackedWidget->addWidget(PrivateMessagesWidget);
    
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

    //Goto Login form -> Register form
    connect(LoginWidget, &Login::registerRequested, this, [=]() {
        if (!RegisterWidget) {
            RegisterWidget = new Register(this);
            stackedWidget->addWidget(RegisterWidget);
        }

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

    //After authentication code is validated, move to Login
    connect(client, &Client::accountAuthenticated, this, [=]() {
        qDebug() << "Clientside account authenticated, moving to login.";
        switchToWidget(stackedWidget->indexOf(LoginWidget));
    });

    //Login -> Private message, mark widgets that wont be used for deletion
    connect(client, &Client::loginSuccess, this, [=]() {
        client->getFriendsList(client->getUser());
        if (!PrivateMessagesWidget) {
            PrivateMessagesWidget = new PrivateMessages(this);
            stackedWidget->addWidget(PrivateMessagesWidget);
        }

        qDebug() << "clientside loggedin";
        switchToWidget(stackedWidget->indexOf(PrivateMessagesWidget));

        //Login, register and auth are done, mark for deletion
        stackedWidget->removeWidget(LoginWidget);
        stackedWidget->removeWidget(RegisterWidget);
        stackedWidget->removeWidget(AuthenticationWidget);

        //deleteLater() is for safe deletion by Qt, if we use 'delete ...Widget;', it might crash if Qt doesnt like it lol
        LoginWidget->deleteLater();
        RegisterWidget->deleteLater();
        AuthenticationWidget->deleteLater();


    });
} //End of Constructor

void ApplicationHandler::switchToWidget(int index){
    if (index >= 0 && index < stackedWidget->count())
    {
        stackedWidget->setCurrentIndex(index);
    }
}
