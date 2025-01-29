// file : main.cpp
// name : eric garcia

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QHostAddress>
#include <QTextStream>
#include <QDebug>
#include <QObject>

#include "Client.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    oom::Client client;
    QHostAddress host = QHostAddress::LocalHost;
    int port = 1234;

    /*
      cmds

      'd' -> display client state
      
      's' -> connect to server
      
      'l' -> login, format is l(usr) (pwd)
             (no parenthesis)

      'c' -> create account, format is c(usr) (pwd)
             (no parenthesis)
    */

    qDebug() << "Commands: (ignore parenthesis)"
             << "\nd            : display client state"
             << "\ns            : connect to server"
             << "\nl(usr) (pwd) : login to account"
             << "\nc(usr) (pwd) : create an account"
             << "\n";
    
    QSocketNotifier * notif = new QSocketNotifier(fileno(stdin),
                                                 QSocketNotifier::Read);
    QObject::connect(notif,&QSocketNotifier::activated,[&client,host,port]() {
        QTextStream qin(stdin);
        QString input = qin.readLine();
        
        if (input[0] == 's')
            client.connectToServer(host, port);
        else if (input[0] == 'l')
        {
            QString x(input); x.remove(0,1);
            QStringList xs = x.split(' ');
            client.login(xs[0],xs[1]);
        }
        else if (input[0] == 'd')
        {
            qDebug() << client.getState();
        }
        else if (input[0] == 'c')
        {
            QString x(input); x.remove(0,1);
            QStringList xs = x.split(' ');
            client.createAccount(xs[0],xs[1]);
        }

    });

  
    
    return app.exec();
}
