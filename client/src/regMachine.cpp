// file : regMachine.cpp
// name : eric garcia

#include "regMachine.h"

RegMachine * RegMachine::instance(NULL);

RegMachine::RegMachine()
    : QObject(NULL), client(Client::getInstance()),
      m(new QStateMachine(this))
{
    QState * idle = new QState();
    idle->setObjectName("idle");
    QState * registering = new QState();
    registering->setObjectName("registering");
    QState * registered = new QState();
    registered->setObjectName("registered");
    QState * authenticating = new QState();
    authenticating->setObjectName("authenticating");
    QState * authenticated = new QState();
    authenticated->setObjectName("authenticated");
    
    
    idle->addTransition(this,&RegMachine::calledCreateAcc,registering);

    registering->addTransition(client, &Client::accountCreated, registered);
    registering->addTransition(client, &Client::accountNotCreated, idle);
    
    registered->addTransition(this,&RegMachine::calledAuthAcc,authenticating);
    
    authenticating->addTransition(client, &Client::accountAuthenticated,
                                  authenticated);
    authenticating->addTransition(client, &Client::accountAuthenticationFail,
                                  registered);
    authenticated->addTransition(this, &RegMachine::finished, idle);

    QObject::connect(idle, &QState::entered, this, [this]
    {
        qDebug() << "Entered idle...";
    });

    QObject::connect(registering, &QState::entered, this, [this]
    {
        qDebug() << "Entered registering...";
    });
    
    QObject::connect(registered, &QState::entered, this, [this]
    {
        qDebug() << "Entered registered...";
    });

    QObject::connect(authenticating, &QState::entered, this, [this]
    {
        qDebug() << "Entered authenticating...";
    });
    
    QObject::connect(authenticated, &QState::entered, this, [this]
    {
        qDebug() << "Entered authenticated...";
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
    qDebug() << "RegMachine createAcc called!!!";
    emit calledCreateAcc();
}

void RegMachine::authAcc(const QString& code)
{
    qDebug() << "RegMachine authAcc called!!!";
    emit calledAuthAcc();
}

void RegMachine::onAuthenticated()
{
    emit finished();
}

