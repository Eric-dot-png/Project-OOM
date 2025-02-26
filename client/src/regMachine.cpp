// file : regMachine.cpp
// name : eric garcia

#include "regMachine.h"

RegMachine * RegMachine::instance(NULL);

RegMachine::RegMachine()
    : QObject(NULL), client(Client::getInstance()),
      m(new QStateMachine(this))
{
    QState * idle = new QState(m);
    QState * registering = new QState(m);
    QState * registered = new QState(m);
    QState * authenticating = new QState(m);
    QState * authenticated = new QState(m);
    
    idle->addTransition(this,&RegMachine::calledCreateAcc,registering);
    QSignalTransition * accSucc = registering->addTransition(
        client, &Client::accountCreated, registered);
    QSignalTransition * accFail = registering->addTransition(
        client, &Client::accountNotCreated, idle);
    registered->addTransition(this,&RegMachine::calledAuthAcc,authenticating);
    
    QSignalTransition * authSucc = authenticating->addTransition(
        client, &Client::accountAuthenticated, authenticated);
    QSignalTransition * authFail = authenticating->addTransition(
        client, &Client::accountAuthenticationFail,registered);
    authenticated->addTransition(this, &RegMachine::finished, idle);
    
    QObject::connect(accSucc, &QSignalTransition::triggered, this, []()
    {
        qDebug() << "Account added to Registration. Now Authenticate:";
    });
    QObject::connect(accFail,&QSignalTransition::triggered,this,[]()
    {
        qDebug() << "Account Was Not Created."
                 << "Try again with different credentials";
    });
    
    QObject::connect(authSucc, &QSignalTransition::triggered, this, []()
    {
        qDebug() << "Account Authenticated. Welcome to OOM!";
    });
    QObject::connect(authFail, &QSignalTransition::triggered, this, []()
    {
        qDebug() << "Account Not Authenticated. Try again.";
    });
    
    
    QObject::connect(authenticated, &QState::entered, this, [this]
    {
        emit finished();
    });

    
    m->setInitialState(idle);
    m->start();
}

RegMachine * RegMachine::getInstance()
{
    if (instance == NULL)
        instance = new RegMachine;
    return instance;
}

void RegMachine::destroyInstance()
{
    if (instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

void RegMachine::createAcc(const User& u)
{
    qDebug() << "attempting to register"
             << u.get_username() << u.get_password()
             << u.get_email();
    client->writeToServer(ProtocolManager::CreateAccountRequest,
                          {u.get_username(), u.get_password(),
                           u.get_email()});
    emit calledCreateAcc();
}

void RegMachine::authAcc(const User& u, const QString& code)
{
    qDebug() << "RegMachine authAcc called!!!";
    client->writeToServer(ProtocolManager::CreateAccountAuthCodeSubmit,
                          {u.get_username(), u.get_password(), code});
    emit calledAuthAcc();
}

void RegMachine::onAuthenticated()
{
    qDebug() << "Gz, account authenticated. welcome!";
    emit finished();
}
