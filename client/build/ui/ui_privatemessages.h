/********************************************************************************
** Form generated from reading UI file 'privatemessages.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATEMESSAGES_H
#define UI_PRIVATEMESSAGES_H

#include <OOMTextBrowser.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateMessages
{
public:
    QTextEdit *textEdit;
    OOMTextBrowser *textBrowser;
    QLabel *friendNameLabel;
    QComboBox *serverCombobox;
    QLineEdit *searchUserTextbox;
    QLabel *label;
    QLabel *userNotFoundLabel;
    QLabel *currentUser;

    void setupUi(QWidget *PrivateMessages)
    {
        if (PrivateMessages->objectName().isEmpty())
            PrivateMessages->setObjectName(QString::fromUtf8("PrivateMessages"));
        PrivateMessages->resize(1244, 625);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PrivateMessages->sizePolicy().hasHeightForWidth());
        PrivateMessages->setSizePolicy(sizePolicy);
        textEdit = new QTextEdit(PrivateMessages);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(340, 480, 481, 41));
        textBrowser = new OOMTextBrowser(PrivateMessages);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(340, 160, 481, 301));
        friendNameLabel = new QLabel(PrivateMessages);
        friendNameLabel->setObjectName(QString::fromUtf8("friendNameLabel"));
        friendNameLabel->setGeometry(QRect(340, 140, 441, 18));
        serverCombobox = new QComboBox(PrivateMessages);
        serverCombobox->setObjectName(QString::fromUtf8("serverCombobox"));
        serverCombobox->setGeometry(QRect(1070, 10, 151, 26));
        searchUserTextbox = new QLineEdit(PrivateMessages);
        searchUserTextbox->setObjectName(QString::fromUtf8("searchUserTextbox"));
        searchUserTextbox->setGeometry(QRect(430, 100, 113, 26));
        label = new QLabel(PrivateMessages);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(340, 100, 81, 20));
        userNotFoundLabel = new QLabel(PrivateMessages);
        userNotFoundLabel->setObjectName(QString::fromUtf8("userNotFoundLabel"));
        userNotFoundLabel->setGeometry(QRect(550, 100, 191, 21));
        userNotFoundLabel->setWordWrap(true);
        currentUser = new QLabel(PrivateMessages);
        currentUser->setObjectName(QString::fromUtf8("currentUser"));
        currentUser->setGeometry(QRect(10, 10, 58, 18));
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
        currentUser->setText(QCoreApplication::translate("PrivateMessages", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateMessages: public Ui_PrivateMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGES_H
