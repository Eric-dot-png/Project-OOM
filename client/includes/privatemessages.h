//Name: Alex Franke
//File: privatemessages.h
//OOM project

/*
 * PrivateMessages Form.
 */

#ifndef PRIVATEMESSAGES_H
#define PRIVATEMESSAGES_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>

#include "oomwidget.h"
#include "Client.h"
#include "ui_privatemessages.h"
#include "dmlistmodel.h"

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

class PrivateMessages : public OOMWidget
{
    Q_OBJECT

public:
    explicit PrivateMessages(QWidget *parent = nullptr);
    ~PrivateMessages();

private slots:
    void onEnterKeyPressed();

    //Takes the json file and parses it
    void loadPage();

    void openDM(const QModelIndex &index);
    
signals:


private:
    Ui::PrivateMessages *ui;
    EnterKeyFilter *enterFilter;
    DMListModel *messagingList;

    //scuffed for now
    User currentlyMessaging;

    QString formatClientMessage();
    QString formatOtherMessage();
    void searchUser();
    void sendFriendRequest();
    void receivedMessage(QString from, QString msg);
    void showAddFriendButton();
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *, QEvent *);

};

#endif // PRIVATEMESSAGES_H
