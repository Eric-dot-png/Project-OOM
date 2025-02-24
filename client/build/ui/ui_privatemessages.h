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
#include <QtWidgets/QLineEdit>
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
    QComboBox *serverCombobox;
    QLineEdit *searchUserTextbox;
    QLabel *label;
    QLabel *userNotFoundLabel;

    void setupUi(QWidget *PrivateMessages)
    {
        if (PrivateMessages->objectName().isEmpty())
            PrivateMessages->setObjectName(QString::fromUtf8("PrivateMessages"));
        PrivateMessages->resize(796, 561);
        textEdit = new QTextEdit(PrivateMessages);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(160, 470, 481, 41));
        textBrowser = new QTextBrowser(PrivateMessages);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(160, 150, 481, 301));
        friendNameLabel = new QLabel(PrivateMessages);
        friendNameLabel->setObjectName(QString::fromUtf8("friendNameLabel"));
        friendNameLabel->setGeometry(QRect(160, 130, 441, 18));
        serverCombobox = new QComboBox(PrivateMessages);
        serverCombobox->setObjectName(QString::fromUtf8("serverCombobox"));
        serverCombobox->setGeometry(QRect(630, 10, 151, 26));
        searchUserTextbox = new QLineEdit(PrivateMessages);
        searchUserTextbox->setObjectName(QString::fromUtf8("searchUserTextbox"));
        searchUserTextbox->setGeometry(QRect(210, 30, 113, 26));
        label = new QLabel(PrivateMessages);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 30, 81, 20));
        userNotFoundLabel = new QLabel(PrivateMessages);
        userNotFoundLabel->setObjectName(QString::fromUtf8("userNotFoundLabel"));
        userNotFoundLabel->setGeometry(QRect(330, 30, 191, 21));
        userNotFoundLabel->setWordWrap(true);
        QWidget::setTabOrder(textEdit, serverCombobox);
        QWidget::setTabOrder(serverCombobox, textBrowser);

        retranslateUi(PrivateMessages);

        QMetaObject::connectSlotsByName(PrivateMessages);
    } // setupUi

    void retranslateUi(QWidget *PrivateMessages)
    {
        PrivateMessages->setWindowTitle(QCoreApplication::translate("PrivateMessages", "Form", nullptr));
        friendNameLabel->setText(QCoreApplication::translate("PrivateMessages", "friendName", nullptr));
#if QT_CONFIG(whatsthis)
        serverCombobox->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        serverCombobox->setPlaceholderText(QCoreApplication::translate("PrivateMessages", "Choose a Server...", nullptr));
        label->setText(QCoreApplication::translate("PrivateMessages", "Search User:", nullptr));
        userNotFoundLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PrivateMessages: public Ui_PrivateMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGES_H
