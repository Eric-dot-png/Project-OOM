#ifndef OOMWIDGET_H
#define OOMWIDGET_H

#include <QWidget>
#include "Client.h"

class OOMWidget : public QWidget {
    Q_OBJECT
public:
    explicit OOMWidget(QWidget *parent = nullptr)
        : QWidget(parent) {}

    void startApp();
    void reloadStyleSheet();

    static void setClient(Client *c) { client = c; }
    static Client *getClient() { return client; }

    QString loadStyleSheet(const QString& resourcePath);

protected:
    static Client *client;
    OOMWidget(const OOMWidget&) = default;
    OOMWidget& operator=(const OOMWidget&) = default;

    QMap<QString, QString> styles {
        {"DarkMode", ":/stylesheets/stylesheets/darkmode.qss"},
        {"LightMode", ":/stylesheets/stylesheets/lightmode.qss"}
    };

};

#endif // OOMWIDGET_H
