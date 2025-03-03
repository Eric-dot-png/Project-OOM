#ifndef CONFIG_H
#define CONFIG_H

#include <QHostAddress>

const bool DEBUG_MODE = true;
const bool LOCAL_MODE = false;

const QHostAddress SERVER_IP = (LOCAL_MODE ? QHostAddress::LocalHost :
                                QHostAddress("216.106.76.23")); // sample ip
const QHostAddress SERVER_HOST_IP = (LOCAL_MODE ? QHostAddress::LocalHost :
                                     QHostAddress::Any)
const int SERVER_PORT = 1234;

#endif 
