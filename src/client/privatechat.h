#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();

private slots:
    void on_sendMsgButton_clicked();

private:
    Ui::PrivateChat *ui;
};

#endif // PRIVATECHAT_H
