/********************************************************************************
** Form generated from reading UI file 'profile.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILE_H
#define UI_PROFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Profile
{
public:
    QLabel *helloLabel;
    QLabel *usernameLabel;

    void setupUi(QWidget *Profile)
    {
        if (Profile->objectName().isEmpty())
            Profile->setObjectName("Profile");
        Profile->resize(640, 480);
        helloLabel = new QLabel(Profile);
        helloLabel->setObjectName("helloLabel");
        helloLabel->setGeometry(QRect(110, 40, 63, 20));
        usernameLabel = new QLabel(Profile);
        usernameLabel->setObjectName("usernameLabel");
        usernameLabel->setGeometry(QRect(150, 40, 63, 20));

        retranslateUi(Profile);

        QMetaObject::connectSlotsByName(Profile);
    } // setupUi

    void retranslateUi(QWidget *Profile)
    {
        Profile->setWindowTitle(QCoreApplication::translate("Profile", "Form", nullptr));
        helloLabel->setText(QCoreApplication::translate("Profile", "Hello, ", nullptr));
        usernameLabel->setText(QCoreApplication::translate("Profile", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Profile: public Ui_Profile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILE_H
