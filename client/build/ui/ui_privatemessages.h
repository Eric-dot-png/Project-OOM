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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
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
    QPushButton *addFriendButton;
    QListWidget *listWidget;
    QCheckBox *styleCheckbox;
    QLabel *loadingLabel;

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
        textEdit->setGeometry(QRect(350, 550, 481, 41));
        textBrowser = new OOMTextBrowser(PrivateMessages);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(350, 160, 481, 371));
        friendNameLabel = new QLabel(PrivateMessages);
        friendNameLabel->setObjectName(QString::fromUtf8("friendNameLabel"));
        friendNameLabel->setGeometry(QRect(350, 117, 441, 41));
        friendCombobox = new QComboBox(PrivateMessages);
        friendCombobox->setObjectName(QString::fromUtf8("friendCombobox"));
        friendCombobox->setGeometry(QRect(190, 10, 151, 26));
        searchUserTextbox = new QLineEdit(PrivateMessages);
        searchUserTextbox->setObjectName(QString::fromUtf8("searchUserTextbox"));
        searchUserTextbox->setGeometry(QRect(460, 50, 113, 26));
        label = new QLabel(PrivateMessages);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(350, 50, 101, 20));
        userNotFoundLabel = new QLabel(PrivateMessages);
        userNotFoundLabel->setObjectName(QString::fromUtf8("userNotFoundLabel"));
        userNotFoundLabel->setGeometry(QRect(580, 10, 211, 101));
        userNotFoundLabel->setWordWrap(true);
        currentUser = new QLabel(PrivateMessages);
        currentUser->setObjectName(QString::fromUtf8("currentUser"));
        currentUser->setGeometry(QRect(350, 17, 101, 21));
        friendView = new QListView(PrivateMessages);
        friendView->setObjectName(QString::fromUtf8("friendView"));
        friendView->setGeometry(QRect(190, 50, 141, 561));
        serverView = new QListView(PrivateMessages);
        serverView->setObjectName(QString::fromUtf8("serverView"));
        serverView->setEnabled(false);
        serverView->setGeometry(QRect(30, 50, 101, 561));
        label_2 = new QLabel(PrivateMessages);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(1050, 0, 191, 41));
        groupBox = new QGroupBox(PrivateMessages);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(800, 0, 191, 131));
        friendRequestComboBox = new QComboBox(groupBox);
        friendRequestComboBox->setObjectName(QString::fromUtf8("friendRequestComboBox"));
        friendRequestComboBox->setGeometry(QRect(40, 40, 121, 26));
        acceptButton = new QPushButton(groupBox);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));
        acceptButton->setGeometry(QRect(20, 90, 51, 26));
        denyButton = new QPushButton(groupBox);
        denyButton->setObjectName(QString::fromUtf8("denyButton"));
        denyButton->setGeometry(QRect(119, 90, 51, 26));
        addFriendButton = new QPushButton(PrivateMessages);
        addFriendButton->setObjectName(QString::fromUtf8("addFriendButton"));
        addFriendButton->setGeometry(QRect(350, 90, 91, 26));
        listWidget = new QListWidget(PrivateMessages);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(980, 420, 256, 192));
        styleCheckbox = new QCheckBox(PrivateMessages);
        styleCheckbox->setObjectName(QString::fromUtf8("styleCheckbox"));
        styleCheckbox->setGeometry(QRect(20, 10, 91, 31));
        loadingLabel = new QLabel(PrivateMessages);
        loadingLabel->setObjectName(QString::fromUtf8("loadingLabel"));
        loadingLabel->setGeometry(QRect(840, 150, 45, 45));
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
        currentUser->setText(QCoreApplication::translate("PrivateMessages", "yourUsername", nullptr));
        label_2->setText(QCoreApplication::translate("PrivateMessages", "Listen for right click", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PrivateMessages", "Pending friend requests", nullptr));
        acceptButton->setText(QCoreApplication::translate("PrivateMessages", "Accept", nullptr));
        denyButton->setText(QCoreApplication::translate("PrivateMessages", "Deny", nullptr));
        addFriendButton->setText(QCoreApplication::translate("PrivateMessages", "+ Add friend", nullptr));
        styleCheckbox->setText(QCoreApplication::translate("PrivateMessages", "Dark Mode", nullptr));
        loadingLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PrivateMessages: public Ui_PrivateMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGES_H
