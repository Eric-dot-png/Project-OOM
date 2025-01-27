#include <iostream>

#include <QDebug>
#include "ProtocolManager.h"


int main()
{
    QString m = ProtocolManager::constructMsg(ProtocolManager::LoginRequest,
                                              2, {"Eric", "123"} );

    qDebug() << m;

    qDebug() << ProtocolManager::classify(m) 
             << ProtocolManager::contents(m);
}
