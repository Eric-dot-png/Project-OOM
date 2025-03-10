// file : config.h
// name : eric garcia

#ifndef CONFIG_H
#define CONFIG_H

#include <QHostAddress>

const bool DEBUG_MODE = true; 
const bool LOCAL_MODE = true;

// what ip the client connects to
const QHostAddress SERVER_IP = (LOCAL_MODE ? QHostAddress::LocalHost :
                                QHostAddress("216.106.76.23")); // sample ip

// what ip the server hosts as
const QHostAddress SERVER_HOST_IP = (LOCAL_MODE ? QHostAddress::LocalHost :
                                     QHostAddress::Any); // any available ip

// ^^^ I don't really understand why they have to be different, but hey, this
// is just how it works. 

const int SERVER_PORT = 1234;

#endif 
