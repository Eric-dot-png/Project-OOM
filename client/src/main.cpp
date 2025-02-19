//Names: Eric Garcia
//       Bethany Spatafora
//       Alex Franke
//File: main.cpp
//OOM project

#include "applicationhandler.h"
#include "Client.h"
#include <QApplication>
#include <QDebug>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &msg)
{
    if (!QString(context.category).startsWith("qt")) {
        QTextStream(stdout) << msg << Qt::endl;
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(customMessageHandler);
    QLoggingCategory::setFilterRules("*.debug=true");
    QApplication a(argc, argv);
    oom::Client * c = oom::Client::getInstance();
    ApplicationHandler oomApp(c);
    oomApp.resize(800, 600);
    oomApp.show();

    int out = a.exec();
    
    oom::Client::destroyInstance();
    return out;
}
