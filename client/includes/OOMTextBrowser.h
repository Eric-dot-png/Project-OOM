#ifndef OOMTEXTBROWSER_H
#define OOMTEXTBROWSER_H

#include <QTextBrowser>
#include <QVector>
#include <QMap>

#include "message.h"

//REMOVE ?

class OOMTextBrowser : public QTextBrowser {
    Q_OBJECT

public:
    explicit OOMTextBrowser(QWidget *parent = nullptr);

    void appendMessage(const Message &msg, bool history);
    void addMessageToHistory(const Message msg);

    const QVector<Message>& getMessages() const { return messageList; }
    void clearHistory() { messageList.clear(); }



private:
    QVector<Message> messageList;
};

#endif //OOMTEXTBROWSER_H
