// file : main.cpp

#include <iostream>
#include <QtCore>
#include <ctime>

#include "Server.h"
#include "dbHandler.h"
#include <mysql.h>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!QString(context.category).startsWith("qt")) {
        QTextStream(stdout) << msg << Qt::endl;
    }
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    qInstallMessageHandler(customMessageHandler);
    QLoggingCategory::setFilterRules("*.debug=true");
    QCoreApplication app(argc, argv);
    int port = 1234; // port for server to listen on
    oom::Server s(port);
    
    return app.exec();
    
    return 0;
}

