#ifndef PRIVATEMESSAGES_H
#define PRIVATEMESSAGES_H

#include <QWidget>
#include <QKeyEvent>
#include "Client.h"
#include "ui_privatemessages.h"

class EnterKeyFilter : public QObject {
    Q_OBJECT
public:
    explicit EnterKeyFilter(QObject* parent = nullptr) : QObject(parent) {}
protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                qDebug() << "Enter key detected.";
                emit enterPressed();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

signals:
    void enterPressed();
};

class PrivateMessages : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateMessages(oom::Client *client, QWidget *parent = nullptr);
    ~PrivateMessages();

private slots:
    void onEnterKeyPressed();
    /*
    void sendMessage();
    void changeFriend();
    void changeServer();*/


signals:


private:
    Ui::PrivateMessages *ui;
    oom::Client *client;
    EnterKeyFilter *enterFilter;
};

#endif // PRIVATEMESSAGES_H
