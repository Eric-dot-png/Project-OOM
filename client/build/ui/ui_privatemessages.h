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
#include <QtWidgets/QUndoView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateMessages
{
public:
    QTextEdit *textEdit;
    OOMTextBrowser *textBrowser;
    QLabel *friendNameLabel;
    QComboBox *friendCombobox;
    QLineEdit *searchUserTextbox;
    QLabel *label;
    QLabel *userNotFoundLabel;
    QLabel *currentUser;
    QListView *friendView;
    QListView *serverView;
    QLabel *label_2;
    QGroupBox *groupBox;
    QComboBox *friendRequestComboBox;
    QPushButton *acceptButton;
    QPushButton *denyButton;
    QUndoView *undoView;
    QPushButton *addFriendButton;

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
        textBrowser->setGeometry(QRect(260, 160, 481, 371));
        friendNameLabel = new QLabel(PrivateMessages);
        friendNameLabel->setObjectName(QString::fromUtf8("friendNameLabel"));
        friendNameLabel->setGeometry(QRect(260, 140, 441, 18));
        friendCombobox = new QComboBox(PrivateMessages);
        friendCombobox->setObjectName(QString::fromUtf8("friendCombobox"));
        friendCombobox->setGeometry(QRect(100, 10, 151, 26));
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
        friendRequestComboBox = new QComboBox(groupBox);
        friendRequestComboBox->setObjectName(QString::fromUtf8("friendRequestComboBox"));
        friendRequestComboBox->setGeometry(QRect(10, 30, 121, 26));
        acceptButton = new QPushButton(groupBox);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));
        acceptButton->setGeometry(QRect(20, 90, 51, 26));
        denyButton = new QPushButton(groupBox);
        denyButton->setObjectName(QString::fromUtf8("denyButton"));
        denyButton->setGeometry(QRect(119, 90, 51, 26));
        undoView = new QUndoView(PrivateMessages);
        undoView->setObjectName(QString::fromUtf8("undoView"));
        undoView->setGeometry(QRect(970, 400, 256, 192));
        addFriendButton = new QPushButton(PrivateMessages);
        addFriendButton->setObjectName(QString::fromUtf8("addFriendButton"));
        addFriendButton->setGeometry(QRect(260, 110, 91, 26));
        QWidget::setTabOrder(textEdit, friendCombobox);
        QWidget::setTabOrder(friendCombobox, textBrowser);

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
        friendCombobox->setPlaceholderText(QCoreApplication::translate("PrivateMessages", "Click to view friends!", nullptr));
        label->setText(QCoreApplication::translate("PrivateMessages", "Search User:", nullptr));
        userNotFoundLabel->setText(QString());
        currentUser->setText(QCoreApplication::translate("PrivateMessages", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("PrivateMessages", "Listen for right click", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PrivateMessages", "Pending friend requests", nullptr));
        acceptButton->setText(QCoreApplication::translate("PrivateMessages", "Accept", nullptr));
        denyButton->setText(QCoreApplication::translate("PrivateMessages", "Deny", nullptr));
        addFriendButton->setText(QCoreApplication::translate("PrivateMessages", "+ Add friend", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateMessages: public Ui_PrivateMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGES_H
