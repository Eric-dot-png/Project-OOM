/********************************************************************************
** Form generated from reading UI file 'privatechat.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATECHAT_H
#define UI_PRIVATECHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateChat
{
public:
    QTextEdit *clientTextbox;
    QPushButton *sendMsgButton;
    QTextBrowser *chatBrowser;

    void setupUi(QWidget *PrivateChat)
    {
        if (PrivateChat->objectName().isEmpty())
            PrivateChat->setObjectName("PrivateChat");
        PrivateChat->resize(640, 480);
        clientTextbox = new QTextEdit(PrivateChat);
        clientTextbox->setObjectName("clientTextbox");
        clientTextbox->setGeometry(QRect(40, 424, 451, 31));
        sendMsgButton = new QPushButton(PrivateChat);
        sendMsgButton->setObjectName("sendMsgButton");
        sendMsgButton->setGeometry(QRect(500, 430, 83, 29));
        chatBrowser = new QTextBrowser(PrivateChat);
        chatBrowser->setObjectName("chatBrowser");
        chatBrowser->setGeometry(QRect(40, 20, 451, 371));

        retranslateUi(PrivateChat);

        QMetaObject::connectSlotsByName(PrivateChat);
    } // setupUi

    void retranslateUi(QWidget *PrivateChat)
    {
        PrivateChat->setWindowTitle(QCoreApplication::translate("PrivateChat", "Form", nullptr));
        sendMsgButton->setText(QCoreApplication::translate("PrivateChat", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateChat: public Ui_PrivateChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATECHAT_H
