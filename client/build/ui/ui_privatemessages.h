/********************************************************************************
** Form generated from reading UI file 'privatemessages.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATEMESSAGES_H
#define UI_PRIVATEMESSAGES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateMessages
{
public:
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    QLabel *friendNameLabel;
    QComboBox *friendCombobox;
    QComboBox *serverCombobox;

    void setupUi(QWidget *PrivateMessages)
    {
        if (PrivateMessages->objectName().isEmpty())
            PrivateMessages->setObjectName(QString::fromUtf8("PrivateMessages"));
        PrivateMessages->resize(640, 480);
        textEdit = new QTextEdit(PrivateMessages);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(110, 390, 481, 41));
        textBrowser = new QTextBrowser(PrivateMessages);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(110, 70, 481, 301));
        friendNameLabel = new QLabel(PrivateMessages);
        friendNameLabel->setObjectName(QString::fromUtf8("friendNameLabel"));
        friendNameLabel->setGeometry(QRect(150, 40, 81, 18));
        friendCombobox = new QComboBox(PrivateMessages);
        friendCombobox->setObjectName(QString::fromUtf8("friendCombobox"));
        friendCombobox->setGeometry(QRect(10, 10, 131, 26));
        serverCombobox = new QComboBox(PrivateMessages);
        serverCombobox->setObjectName(QString::fromUtf8("serverCombobox"));
        serverCombobox->setGeometry(QRect(470, 10, 121, 26));
        QWidget::setTabOrder(textEdit, friendCombobox);
        QWidget::setTabOrder(friendCombobox, serverCombobox);
        QWidget::setTabOrder(serverCombobox, textBrowser);

        retranslateUi(PrivateMessages);

        QMetaObject::connectSlotsByName(PrivateMessages);
    } // setupUi

    void retranslateUi(QWidget *PrivateMessages)
    {
        PrivateMessages->setWindowTitle(QCoreApplication::translate("PrivateMessages", "Form", nullptr));
        friendNameLabel->setText(QCoreApplication::translate("PrivateMessages", "friendName", nullptr));
#if QT_CONFIG(whatsthis)
        friendCombobox->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(whatsthis)
        serverCombobox->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
    } // retranslateUi

};

namespace Ui {
    class PrivateMessages: public Ui_PrivateMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGES_H
