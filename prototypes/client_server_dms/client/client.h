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
        Client(QObject * parent=NULL);
        ~Client();
        void connectToServer(const QHostAddress& host, int port);
                    
    public slots:
        void sendMsg();
        
    private:
        void onServerReply();
        
        QTcpSocket * socket;
        QSocketNotifier * inNotif; // detects stdin (input)
    };
};

#endif
