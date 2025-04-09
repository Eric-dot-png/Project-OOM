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

#include "TextGui.h"

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
        QTextStream(stdout) << "DEBUG: " << msg << Qt::endl;
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

    bool textMode = false; // text gui or not
    
    //Main app
    QApplication app(argc, argv);
    Client * c = Client::getInstance();
    OOMWidget * mainWidget = NULL;
    TextGui * gui = NULL;
    if (textMode)
    {
        gui = new TextGui();
    }
    else
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        applyStyleSheet(":/stylesheets/stylesheets/lightmode.qss");
        
        OOMWidget::setClient(c);
        
        mainWidget = new OOMWidget();
        mainWidget->startApp();
    }
    
    int out = app.exec();

    if (!textMode)
    {
        delete mainWidget;
    }
    else
    {
        delete gui;
    }
    
    Client::destroyInstance();
    Serializers::destroyInstances();
    
    return out;
}
