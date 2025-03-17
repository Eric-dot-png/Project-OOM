// file : main.cpp
//
// Description: This is the main entry point for the server-side application in a Discord-like
//              project. It sets up logging (if in debug mode), initializes the Qt event loop,
//              creates the server instance, and handles cleanup when the application exits.

#include <iostream>
#include <QtCore>
#include <ctime>

#include "Server.h"
#include "dbHandler.h"
#include <mysql.h>

#include "config.h"

// Custom message handler to filter out certain Qt debug messages
// in order to keep console logs cleaner. Only messages not prefixed
// with "qt" are printed to standard output.
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!QString(context.category).startsWith("qt")) {
        QTextStream(stdout) << msg << Qt::endl;
    }
}

int main(int argc, char* argv[])
{
    // Seed the random number generator with the current time
    srand(time(NULL));

    // If the build is in debug mode, install a custom message handler and enable debug logging
    if (DEBUG_MODE)
    {
        qInstallMessageHandler(customMessageHandler);
        QLoggingCategory::setFilterRules("*.debug=true");
    }

    // Create a QCoreApplication for non-GUI event loop support
    QCoreApplication app(argc, argv);

    // Instantiate the server singleton, which starts listening for incoming connections
    Server::getInstance();

    // Enter the Qt event loop, which keeps the application running until exit
    int out = app.exec();

    // Once the event loop ends, perform necessary cleanup
    dbHandler::destroyInstance();      // Clean up database handler resources
    Server::destroyInstance();         // Destroy the server singleton
    Serializers::destroyInstances();   // Clean up any serializer singletons

    // Return the exit code from the Qt application
    return out;
}