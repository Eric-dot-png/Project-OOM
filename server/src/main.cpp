// file : main.cpp

#include <iostream>
#include <QtCore>
#include <ctime>

#include "Server.h"
#include "dbHandler.h"
#include <mysql.h>

#include "config.h"

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!QString(context.category).startsWith("qt")) {
        QTextStream(stdout) << msg << Qt::endl;
    }
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    if (DEBUG_MODE)
    {
        qInstallMessageHandler(customMessageHandler);
        QLoggingCategory::setFilterRules("*.debug=true");
    }
    
    QCoreApplication app(argc, argv);
    Server::getInstance();    
    int out = app.exec();
    
    dbHandler::destroyInstance();
    Server::destroyInstance();
    return out;
}

