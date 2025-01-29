/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QLineEdit *usrnameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *emailEdit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *registerButton;
    QPushButton *backButton;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName("Register");
        Register->resize(640, 480);
        usrnameEdit = new QLineEdit(Register);
        usrnameEdit->setObjectName("usrnameEdit");
        usrnameEdit->setGeometry(QRect(90, 10, 113, 28));
        passwordEdit = new QLineEdit(Register);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(90, 60, 113, 28));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        emailEdit = new QLineEdit(Register);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setGeometry(QRect(90, 110, 113, 28));
        label = new QLabel(Register);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 71, 20));
        label_2 = new QLabel(Register);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 60, 63, 20));
        label_3 = new QLabel(Register);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 120, 63, 20));
        label_4 = new QLabel(Register);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(310, 60, 63, 20));
        registerButton = new QPushButton(Register);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(130, 180, 83, 29));
        backButton = new QPushButton(Register);
        backButton->setObjectName("backButton");
        backButton->setGeometry(QRect(10, 180, 83, 29));

        retranslateUi(Register);

        registerButton->setDefault(true);


        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "Form", nullptr));
        label->setText(QCoreApplication::translate("Register", "Username", nullptr));
        label_2->setText(QCoreApplication::translate("Register", "Password", nullptr));
        label_3->setText(QCoreApplication::translate("Register", "Email", nullptr));
        label_4->setText(QCoreApplication::translate("Register", "TextLabel", nullptr));
        registerButton->setText(QCoreApplication::translate("Register", "Register", nullptr));
        backButton->setText(QCoreApplication::translate("Register", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
