//Name: Alex Franke
//File: privatemessages.h
//OOM project

/*
 * PrivateMessages Form.
 * Currently not used.
 */

#ifndef PRIVATEMESSAGES_H
#define PRIVATEMESSAGES_H

#include <QWidget>
#include <QKeyEvent>
#include "Client.h"
#include "ui_privatemessages.h"
#include "message.h"


/*
 *  QT does not natively allow pressing enter to submit a textbox.
 *  This class checks for button press events and if the enter button
 *  is pressed, it will send a signal to submit messages.
 */
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

    //Takes the json file and parses it
    void loadPage();
    /*
    void sendMessage();
    void changeFriend();
    void changeServer();*/


signals:


private:
    Ui::PrivateMessages *ui;
    oom::Client *client;
    EnterKeyFilter *enterFilter;

    QString formatMessage();

    //void unpackJson();
};

#endif // PRIVATEMESSAGES_H
