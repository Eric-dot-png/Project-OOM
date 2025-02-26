// file : regMachine.h
// name : eric garcia

#ifndef REGMACHINE_H
#define REGMACHINE_H

#include <QObject>
#include <QStateMachine>

#include "Client.h"

class RegMachine : public QObject
{
    Q_OBJECT
    
public:
    RegMachine(const RegMachine&) = delete;
    RegMachine & operator=(const RegMachine&) = delete;

    static RegMachine * getInstance();
    static void destroyInstance();

    void createAcc(const User& u);
    void authAcc(const QString& code);

    void print_state()
    {
        for (const QAbstractState * state : m->configuration())
            qDebug() << "Active state:" << state->objectName();
    }
    
signals:
    void calledCreateAcc();
    void calledAuthAcc();
    void finished();
                   
private slots:
    void onAuthenticated();
    
private:
    RegMachine();
    ~RegMachine() = default;
    
    
    Client * const client;
    QStateMachine * m;
    
    static RegMachine * instance;
};

#endif 
