//Names: Eric Garcia
//       Bethany Spatafora
//       Alex Franke
//File: main.cpp
//OOM project

#include <QApplication>
#include <QStyleFactory>
#include <QDebug>
#include <QMetaType>

#include "config.h"
#include "applicationhandler.h"
#include "oomwidget.h"
#include "Client.h"
#include "regMachine.h"

void applyStyleSheet(const QString &styleSheetPath) {
    QFile file(styleSheetPath);
    qDebug() << "Resource Path Test:" << QFile(":/stylesheets/stylesheets/lightmode.qss").exists();
    if (!file.exists()) {
        qDebug() << "Stylesheet file does not exist:" << styleSheetPath;
        return;
    }


    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        qApp->setStyleSheet(stream.readAll());
        file.close();
    } else {
        qDebug() << "Failed to load stylesheet:" << styleSheetPath;
    }
}

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &msg)
{
    if (!QString(context.category).startsWith("qt")) {
        QTextStream(stdout) << msg << Qt::endl;
    }
}

void declareOOMtypes()
{
    qRegisterMetaType<Message>("Message");
    qRegisterMetaType<QList<Message>>("QList<Message>");
    qRegisterMetaType<User>("User");
}

int main(int argc, char *argv[])
{

    if (DEBUG_MODE)
    {
        // Show debug information
        qInstallMessageHandler(customMessageHandler);
        QLoggingCategory::setFilterRules("*.debug=true");
    }
    
    declareOOMtypes();

    //Main app
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    applyStyleSheet(":/stylesheets/stylesheets/lightmode.qss");
    
    Client * c = Client::getInstance();
    OOMWidget::setClient(c);
    
    OOMWidget *mainWidget = new OOMWidget();
    mainWidget->startApp();

    RegMachine::getInstance();
    
    
    int out = a.exec();

    delete mainWidget;
    Client::destroyInstance();
    Serializers::destroyInstances();
    
    return out;
}
