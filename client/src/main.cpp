//Names: Eric Garcia
//       Bethany Spatafora
//       Alex Franke
//File: main.cpp
//OOM project

#include "applicationhandler.h"
#include "oomwidget.h"
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
    //Show debug information
    qInstallMessageHandler(customMessageHandler);
    QLoggingCategory::setFilterRules("*.debug=true");

    //Main app
    QApplication a(argc, argv);

    Client * c = Client::getInstance();
    OOMWidget::setClient(c);

    OOMWidget *mainWidget = new OOMWidget();
    mainWidget->startApp();

    int out = a.exec();

    Client::destroyInstance();
    return out;
}
