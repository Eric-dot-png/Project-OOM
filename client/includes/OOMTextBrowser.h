#ifndef OOMTEXTBROWSER_H
#define OOMTEXTBROWSER_H

#include <QTextBrowser>
#include <QVector>
#include <QMap>

#include "message.h"

class OOMTextBrowser : public QTextBrowser {
    Q_OBJECT

public:
    explicit OOMTextBrowser(QWidget *parent = nullptr);

    void appendMessage(Message msg);

    const QVector<QMap<QString, QString>>& getMessages() const { return messageList; }

private:
    QVector<QMap<QString, QString>> messageList;
};

#endif //OOMTEXTBROWSER_H
