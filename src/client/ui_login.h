/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *registerNotImplementedLabel;
    QLabel *authFailedLabel;
    QLineEdit *passwordTextbox;
    QLineEdit *usernameTextbox;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(800, 600);
        label = new QLabel(Login);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(240, 230, 81, 20));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(240, 280, 91, 20));
        label_3 = new QLabel(Login);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(240, 330, 271, 20));
        loginButton = new QPushButton(Login);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(470, 280, 83, 29));
        registerButton = new QPushButton(Login);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));
        registerButton->setGeometry(QRect(310, 360, 83, 29));
        registerNotImplementedLabel = new QLabel(Login);
        registerNotImplementedLabel->setObjectName(QString::fromUtf8("registerNotImplementedLabel"));
        registerNotImplementedLabel->setEnabled(false);
        registerNotImplementedLabel->setGeometry(QRect(280, 400, 151, 20));
        authFailedLabel = new QLabel(Login);
        authFailedLabel->setObjectName(QString::fromUtf8("authFailedLabel"));
        authFailedLabel->setEnabled(false);
        authFailedLabel->setGeometry(QRect(460, 230, 231, 41));
        authFailedLabel->setCursor(QCursor(Qt::ArrowCursor));
        authFailedLabel->setWordWrap(true);
        passwordTextbox = new QLineEdit(Login);
        passwordTextbox->setObjectName(QString::fromUtf8("passwordTextbox"));
        passwordTextbox->setGeometry(QRect(320, 280, 113, 28));
        passwordTextbox->setEchoMode(QLineEdit::EchoMode::Password);
        usernameTextbox = new QLineEdit(Login);
        usernameTextbox->setObjectName(QString::fromUtf8("usernameTextbox"));
        usernameTextbox->setGeometry(QRect(320, 230, 113, 28));
        usernameTextbox->setClearButtonEnabled(false);
        usernameTextbox->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        loginButton->raise();
        registerButton->raise();
        registerNotImplementedLabel->raise();
        authFailedLabel->raise();
        passwordTextbox->raise();
        QWidget::setTabOrder(usernameTextbox, passwordTextbox);
        QWidget::setTabOrder(passwordTextbox, loginButton);
        QWidget::setTabOrder(loginButton, registerButton);

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Login", nullptr));
        label->setText(QCoreApplication::translate("Login", "Username:", nullptr));
        label_2->setText(QCoreApplication::translate("Login", "Password:", nullptr));
        label_3->setText(QCoreApplication::translate("Login", "Don't have an account? Register now!", nullptr));
        loginButton->setText(QCoreApplication::translate("Login", "Login", nullptr));
        registerButton->setText(QCoreApplication::translate("Login", "Register", nullptr));
        registerNotImplementedLabel->setText(QString());
        authFailedLabel->setText(QString());
        passwordTextbox->setInputMask(QString());
        passwordTextbox->setPlaceholderText(QCoreApplication::translate("Login", "Password", nullptr));
        usernameTextbox->setPlaceholderText(QCoreApplication::translate("Login", "Username", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
