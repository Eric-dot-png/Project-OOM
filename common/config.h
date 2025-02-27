#ifndef CONFIG_H
#define CONFIG_H

#include <QHostAddress>

const bool DEBUG_MODE = true;
const bool LOCAL_MODE = true;

const QHostAddress SERVER_IP = (LOCAL_MODE ? QHostAddress::LocalHost :
                                QHostAddress("192.168.1.1")); // sample ip
const int SERVER_PORT = 1234;

#endif 
