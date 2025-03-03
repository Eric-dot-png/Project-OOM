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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
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
    QListView *friendView;
    QListView *serverView;
    QLabel *label_2;
    QGroupBox *groupBox;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *dd;

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
        textEdit->setGeometry(QRect(260, 550, 481, 41));
        textBrowser = new OOMTextBrowser(PrivateMessages);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(260, 110, 481, 421));
        friendNameLabel = new QLabel(PrivateMessages);
        friendNameLabel->setObjectName(QString::fromUtf8("friendNameLabel"));
        friendNameLabel->setGeometry(QRect(260, 90, 441, 18));
        serverCombobox = new QComboBox(PrivateMessages);
        serverCombobox->setObjectName(QString::fromUtf8("serverCombobox"));
        serverCombobox->setGeometry(QRect(100, 10, 151, 26));
        searchUserTextbox = new QLineEdit(PrivateMessages);
        searchUserTextbox->setObjectName(QString::fromUtf8("searchUserTextbox"));
        searchUserTextbox->setGeometry(QRect(350, 50, 113, 26));
        label = new QLabel(PrivateMessages);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 50, 81, 20));
        userNotFoundLabel = new QLabel(PrivateMessages);
        userNotFoundLabel->setObjectName(QString::fromUtf8("userNotFoundLabel"));
        userNotFoundLabel->setGeometry(QRect(470, 50, 191, 21));
        userNotFoundLabel->setWordWrap(true);
        currentUser = new QLabel(PrivateMessages);
        currentUser->setObjectName(QString::fromUtf8("currentUser"));
        currentUser->setGeometry(QRect(260, 20, 58, 18));
        friendView = new QListView(PrivateMessages);
        friendView->setObjectName(QString::fromUtf8("friendView"));
        friendView->setGeometry(QRect(100, 50, 151, 561));
        serverView = new QListView(PrivateMessages);
        serverView->setObjectName(QString::fromUtf8("serverView"));
        serverView->setGeometry(QRect(0, 50, 101, 561));
        label_2 = new QLabel(PrivateMessages);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 160, 191, 41));
        groupBox = new QGroupBox(PrivateMessages);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(970, 70, 191, 131));
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 30, 121, 26));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 90, 51, 26));
        dd = new QPushButton(groupBox);
        dd->setObjectName(QString::fromUtf8("dd"));
        dd->setGeometry(QRect(119, 90, 51, 26));
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
        label_2->setText(QCoreApplication::translate("PrivateMessages", "Listen for right click", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PrivateMessages", "Pending friend requests", nullptr));
        pushButton->setText(QCoreApplication::translate("PrivateMessages", "Accept", nullptr));
        dd->setText(QCoreApplication::translate("PrivateMessages", "Deny", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateMessages: public Ui_PrivateMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGES_H
