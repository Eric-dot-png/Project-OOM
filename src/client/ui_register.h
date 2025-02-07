/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
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
    QLabel *testLabel;
    QPushButton *createAccountButton;
    QPushButton *backToLoginButton;
    QLabel *label_4;
    QLineEdit *confirmPasswordEdit;
    QLabel *emailConfirmLabel;
    QLabel *passwordMatchLabel;
    QPushButton *showPasswordButton;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QString::fromUtf8("Register"));
        Register->resize(978, 549);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Register->sizePolicy().hasHeightForWidth());
        Register->setSizePolicy(sizePolicy);
        usrnameEdit = new QLineEdit(Register);
        usrnameEdit->setObjectName(QString::fromUtf8("usrnameEdit"));
        usrnameEdit->setGeometry(QRect(250, 120, 113, 28));
        sizePolicy.setHeightForWidth(usrnameEdit->sizePolicy().hasHeightForWidth());
        usrnameEdit->setSizePolicy(sizePolicy);
        passwordEdit = new QLineEdit(Register);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setGeometry(QRect(250, 170, 113, 28));
        sizePolicy.setHeightForWidth(passwordEdit->sizePolicy().hasHeightForWidth());
        passwordEdit->setSizePolicy(sizePolicy);
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        emailEdit = new QLineEdit(Register);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));
        emailEdit->setGeometry(QRect(250, 260, 113, 28));
        sizePolicy.setHeightForWidth(emailEdit->sizePolicy().hasHeightForWidth());
        emailEdit->setSizePolicy(sizePolicy);
        label = new QLabel(Register);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 120, 71, 20));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label_2 = new QLabel(Register);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 170, 63, 20));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_3 = new QLabel(Register);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(170, 270, 63, 20));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        testLabel = new QLabel(Register);
        testLabel->setObjectName(QString::fromUtf8("testLabel"));
        testLabel->setGeometry(QRect(800, 40, 63, 20));
        sizePolicy.setHeightForWidth(testLabel->sizePolicy().hasHeightForWidth());
        testLabel->setSizePolicy(sizePolicy);
        createAccountButton = new QPushButton(Register);
        createAccountButton->setObjectName(QString::fromUtf8("createAccountButton"));
        createAccountButton->setGeometry(QRect(290, 330, 83, 29));
        sizePolicy.setHeightForWidth(createAccountButton->sizePolicy().hasHeightForWidth());
        createAccountButton->setSizePolicy(sizePolicy);
        backToLoginButton = new QPushButton(Register);
        backToLoginButton->setObjectName(QString::fromUtf8("backToLoginButton"));
        backToLoginButton->setGeometry(QRect(170, 330, 83, 29));
        sizePolicy.setHeightForWidth(backToLoginButton->sizePolicy().hasHeightForWidth());
        backToLoginButton->setSizePolicy(sizePolicy);
        label_4 = new QLabel(Register);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(170, 200, 58, 61));
        label_4->setWordWrap(true);
        confirmPasswordEdit = new QLineEdit(Register);
        confirmPasswordEdit->setObjectName(QString::fromUtf8("confirmPasswordEdit"));
        confirmPasswordEdit->setGeometry(QRect(250, 220, 113, 26));
        sizePolicy.setHeightForWidth(confirmPasswordEdit->sizePolicy().hasHeightForWidth());
        confirmPasswordEdit->setSizePolicy(sizePolicy);
        confirmPasswordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        emailConfirmLabel = new QLabel(Register);
        emailConfirmLabel->setObjectName(QString::fromUtf8("emailConfirmLabel"));
        emailConfirmLabel->setGeometry(QRect(380, 260, 91, 31));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(emailConfirmLabel->sizePolicy().hasHeightForWidth());
        emailConfirmLabel->setSizePolicy(sizePolicy1);
        emailConfirmLabel->setWordWrap(true);
        passwordMatchLabel = new QLabel(Register);
        passwordMatchLabel->setObjectName(QString::fromUtf8("passwordMatchLabel"));
        passwordMatchLabel->setGeometry(QRect(430, 180, 231, 61));
        sizePolicy1.setHeightForWidth(passwordMatchLabel->sizePolicy().hasHeightForWidth());
        passwordMatchLabel->setSizePolicy(sizePolicy1);
        passwordMatchLabel->setWordWrap(true);
        showPasswordButton = new QPushButton(Register);
        showPasswordButton->setObjectName(QString::fromUtf8("showPasswordButton"));
        showPasswordButton->setGeometry(QRect(370, 170, 41, 26));
        QWidget::setTabOrder(usrnameEdit, passwordEdit);
        QWidget::setTabOrder(passwordEdit, confirmPasswordEdit);
        QWidget::setTabOrder(confirmPasswordEdit, emailEdit);
        QWidget::setTabOrder(emailEdit, createAccountButton);
        QWidget::setTabOrder(createAccountButton, backToLoginButton);

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
        testLabel->setText(QCoreApplication::translate("Register", "Test label", nullptr));
        createAccountButton->setText(QCoreApplication::translate("Register", "Register", nullptr));
        backToLoginButton->setText(QCoreApplication::translate("Register", "Back", nullptr));
        label_4->setText(QCoreApplication::translate("Register", "Confirm password", nullptr));
        emailConfirmLabel->setText(QString());
        passwordMatchLabel->setText(QString());
        showPasswordButton->setText(QCoreApplication::translate("Register", "Show", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
