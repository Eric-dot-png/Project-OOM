// file : client.h
// name : eric garcia

#ifndef CLIENT_H
#define CLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QSocketNotifier>
#include <QObject>
#include <QTextStream>
#include <QDebug>

namespace oom
{
    class Client : public QObject
    {
        Q_OBJECT

    public:
        Client(int port, QObject * parent=NULL);
        ~Client();


                 
    public slots:
        void readInput();
        
    private:
        QTcpSocket * socket;
        QSocketNotifier * stdinNotifier; // how reads input
    };
};

#endif
