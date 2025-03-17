// file : regMachine.cpp
// name : eric garcia
// Description: This file implements the RegMachine class, which manages user registration and
//              authentication for a Discord-like Qt-based application. It interacts with the
//              Client class to send registration and authentication requests to the server, and
//              uses QStateMachine to handle the different states of registration.

#include "regMachine.h"

// Singleton instance of RegMachine
RegMachine * RegMachine::instance(NULL);

// Constructor: Initializes the registration state machine and the relevant states.
//              Sets up transitions that occur when certain signals are emitted.
RegMachine::RegMachine()
    : QObject(NULL), client(Client::getInstance()),
      m(new QStateMachine(this))
{
    // Define the QStates used by the state machine.
    QState * idle = new QState(m);
    QState * registering = new QState(m);
    QState * registered = new QState(m);
    QState * authenticating = new QState(m);
    QState * authenticated = new QState(m);

    // Transitions from idle to registering when createAcc is called.
    idle->addTransition(this,&RegMachine::calledCreateAcc,registering);

    // If the Client emits accountCreated, transition from registering to registered.
    QSignalTransition * accSucc = registering->addTransition(
        client, &Client::accountCreated, registered);
    // If the Client emits accountNotCreated, transition from registering back to idle.
    QSignalTransition * accFail = registering->addTransition(
        client, &Client::accountNotCreated, idle);

    // From registered to authenticating when authAcc is called.
    registered->addTransition(this,&RegMachine::calledAuthAcc,authenticating);

    // If the Client emits accountAuthenticated, transition from authenticating to authenticated.
    QSignalTransition * authSucc = authenticating->addTransition(
        client, &Client::accountAuthenticated, authenticated);
    // If the Client emits accountAuthenticationFail, transition from authenticating back to registered.
    QSignalTransition * authFail = authenticating->addTransition(
        client, &Client::accountAuthenticationFail, registered);

    // Once authenticated, transition back to idle when finished signal is emitted.
    authenticated->addTransition(this, &RegMachine::finished, idle);

    // Connect transitions to debug messages for successful or failed account creation.
    QObject::connect(accSucc, &QSignalTransition::triggered, this, []()
    {
        qDebug() << "Account added to Registration. Now Authenticate:";
    });
    QObject::connect(accFail,&QSignalTransition::triggered,this,[]()
    {
        qDebug() << "Account Was Not Created."
                 << "Try again with different credentials";
    });

    // Connect transitions to debug messages for successful or failed authentication.
    QObject::connect(authSucc, &QSignalTransition::triggered, this, []()
    {
        qDebug() << "Account Authenticated. Welcome to OOM!";
    });
    QObject::connect(authFail, &QSignalTransition::triggered, this, []()
    {
        qDebug() << "Account Not Authenticated. Try again.";
    });

    // When entering the authenticated state, emit the finished signal to return to idle.
    QObject::connect(authenticated, &QState::entered, this, [this]
    {
        emit finished();
    });

    // Set the initial state of the machine to idle and start the machine.
    m->setInitialState(idle);
    m->start();
}

// Returns the singleton instance of RegMachine, creating it if needed.
RegMachine * RegMachine::getInstance()
{
    if (instance == NULL)
        instance = new RegMachine;
    return instance;
}

// Destroys the singleton instance, deleting it and resetting it to NULL.
void RegMachine::destroyInstance()
{
    if (instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

// createAcc: Sends a request to create a new account for the given user,
//            then emits a signal to transition from idle to registering state.
void RegMachine::createAcc(const User& u)
{
    qDebug() << "attempting to register"
             << u.get_username() << u.get_password()
             << u.get_email();
    client->writeToServer(Protocol::CreateAccountRequest,
                          {u.get_username(), u.get_password(),
                           u.get_email()});
    emit calledCreateAcc();
}

// authAcc: Sends an authentication code to the server for the given user,
//          then emits a signal to transition from registered to authenticating state.
void RegMachine::authAcc(const User& u, const QString& code)
{
    qDebug() << "RegMachine authAcc called!!!";
    client->writeToServer(Protocol::AuthCodeSubmit,
                          {u.get_username(), u.get_password(), code});
    emit calledAuthAcc();
}

// onAuthenticated: Invoked when the account is successfully authenticated.
//                  Emits the finished signal to transition from authenticated back to idle.
void RegMachine::onAuthenticated()
{
    qDebug() << "Gz, account authenticated. welcome!";
    emit finished();
}