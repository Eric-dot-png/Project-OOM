/********************************************************************************
** Form generated from reading UI file 'groupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPDIALOG_H
#define UI_GROUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_groupDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *checkBoxLayout;
    QPushButton *closeButton;
    QPushButton *createButton;
    QLabel *errorLabel;
    QLabel *label_2;
    QLineEdit *groupNameTextbox;

    void setupUi(QDialog *groupDialog)
    {
        if (groupDialog->objectName().isEmpty())
            groupDialog->setObjectName(QString::fromUtf8("groupDialog"));
        groupDialog->resize(640, 480);
        verticalLayoutWidget = new QWidget(groupDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(210, 110, 160, 221));
        checkBoxLayout = new QVBoxLayout(verticalLayoutWidget);
        checkBoxLayout->setObjectName(QString::fromUtf8("checkBoxLayout"));
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);
        closeButton = new QPushButton(groupDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(160, 360, 80, 31));
        createButton = new QPushButton(groupDialog);
        createButton->setObjectName(QString::fromUtf8("createButton"));
        createButton->setGeometry(QRect(340, 360, 101, 31));
        errorLabel = new QLabel(groupDialog);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setGeometry(QRect(180, 10, 231, 41));
        label_2 = new QLabel(groupDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(77, 59, 101, 31));
        groupNameTextbox = new QLineEdit(groupDialog);
        groupNameTextbox->setObjectName(QString::fromUtf8("groupNameTextbox"));
        groupNameTextbox->setGeometry(QRect(190, 60, 221, 31));

        retranslateUi(groupDialog);

        QMetaObject::connectSlotsByName(groupDialog);
    } // setupUi

    void retranslateUi(QDialog *groupDialog)
    {
        groupDialog->setWindowTitle(QCoreApplication::translate("groupDialog", "Dialog", nullptr));
        closeButton->setText(QCoreApplication::translate("groupDialog", "Close", nullptr));
        createButton->setText(QCoreApplication::translate("groupDialog", "Create Group", nullptr));
        errorLabel->setText(QCoreApplication::translate("groupDialog", "Choose friends to add to the group", nullptr));
        label_2->setText(QCoreApplication::translate("groupDialog", "Group Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class groupDialog: public Ui_groupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPDIALOG_H
