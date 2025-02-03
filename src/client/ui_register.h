/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
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
    QPushButton *createAccountButton;
    QPushButton *backToLoginButton;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName("Register");
        Register->resize(978, 549);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Register->sizePolicy().hasHeightForWidth());
        Register->setSizePolicy(sizePolicy);
        usrnameEdit = new QLineEdit(Register);
        usrnameEdit->setObjectName("usrnameEdit");
        usrnameEdit->setGeometry(QRect(250, 120, 113, 28));
        sizePolicy.setHeightForWidth(usrnameEdit->sizePolicy().hasHeightForWidth());
        usrnameEdit->setSizePolicy(sizePolicy);
        passwordEdit = new QLineEdit(Register);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(250, 170, 113, 28));
        sizePolicy.setHeightForWidth(passwordEdit->sizePolicy().hasHeightForWidth());
        passwordEdit->setSizePolicy(sizePolicy);
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        emailEdit = new QLineEdit(Register);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setGeometry(QRect(250, 220, 113, 28));
        sizePolicy.setHeightForWidth(emailEdit->sizePolicy().hasHeightForWidth());
        emailEdit->setSizePolicy(sizePolicy);
        label = new QLabel(Register);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 120, 71, 20));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label_2 = new QLabel(Register);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(170, 170, 63, 20));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_3 = new QLabel(Register);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(170, 230, 63, 20));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_4 = new QLabel(Register);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(470, 170, 63, 20));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        createAccountButton = new QPushButton(Register);
        createAccountButton->setObjectName("createAccountButton");
        createAccountButton->setGeometry(QRect(290, 290, 83, 29));
        sizePolicy.setHeightForWidth(createAccountButton->sizePolicy().hasHeightForWidth());
        createAccountButton->setSizePolicy(sizePolicy);
        backToLoginButton = new QPushButton(Register);
        backToLoginButton->setObjectName("backToLoginButton");
        backToLoginButton->setGeometry(QRect(170, 290, 83, 29));
        sizePolicy.setHeightForWidth(backToLoginButton->sizePolicy().hasHeightForWidth());
        backToLoginButton->setSizePolicy(sizePolicy);

        retranslateUi(Register);

        createAccountButton->setDefault(true);


        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "Login", nullptr));
        label->setText(QCoreApplication::translate("Register", "Username", nullptr));
        label_2->setText(QCoreApplication::translate("Register", "Password", nullptr));
        label_3->setText(QCoreApplication::translate("Register", "Email", nullptr));
        label_4->setText(QCoreApplication::translate("Register", "TextLabel", nullptr));
        createAccountButton->setText(QCoreApplication::translate("Register", "Register", nullptr));
        backToLoginButton->setText(QCoreApplication::translate("Register", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
