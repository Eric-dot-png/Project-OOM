#ifndef OOMWIDGET_H
#define OOMWIDGET_H

#include <QWidget>
#include "Client.h"

class OOMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OOMWidget(QWidget *parent = nullptr)
        : QWidget(parent) {}

    void startApp();

    static void setClient(Client *c) { client = c; }
    static Client *getClient() { return client; }

protected:
    static Client *client;

};

#endif // OOMWIDGET_H
