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
    QLabel *loadingLabel;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(800, 600);
        label = new QLabel(Login);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(220, 230, 91, 20));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(220, 280, 91, 20));
        label_3 = new QLabel(Login);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(240, 320, 271, 31));
        loginButton = new QPushButton(Login);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(470, 280, 83, 31));
        loginButton->setCursor(QCursor(Qt::PointingHandCursor));
        registerButton = new QPushButton(Login);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));
        registerButton->setGeometry(QRect(310, 360, 83, 31));
        registerButton->setCursor(QCursor(Qt::PointingHandCursor));
        registerNotImplementedLabel = new QLabel(Login);
        registerNotImplementedLabel->setObjectName(QString::fromUtf8("registerNotImplementedLabel"));
        registerNotImplementedLabel->setEnabled(false);
        registerNotImplementedLabel->setGeometry(QRect(280, 400, 151, 20));
        authFailedLabel = new QLabel(Login);
        authFailedLabel->setObjectName(QString::fromUtf8("authFailedLabel"));
        authFailedLabel->setEnabled(false);
        authFailedLabel->setGeometry(QRect(460, 200, 231, 71));
        authFailedLabel->setCursor(QCursor(Qt::ArrowCursor));
        authFailedLabel->setWordWrap(true);
        passwordTextbox = new QLineEdit(Login);
        passwordTextbox->setObjectName(QString::fromUtf8("passwordTextbox"));
        passwordTextbox->setGeometry(QRect(320, 280, 113, 28));
        passwordTextbox->setMaxLength(30);
        passwordTextbox->setEchoMode(QLineEdit::Password);
        usernameTextbox = new QLineEdit(Login);
        usernameTextbox->setObjectName(QString::fromUtf8("usernameTextbox"));
        usernameTextbox->setGeometry(QRect(320, 230, 113, 28));
        usernameTextbox->setMaxLength(20);
        usernameTextbox->setClearButtonEnabled(false);
        loadingLabel = new QLabel(Login);
        loadingLabel->setObjectName(QString::fromUtf8("loadingLabel"));
        loadingLabel->setGeometry(QRect(560, 270, 45, 45));
        usernameTextbox->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        loginButton->raise();
        registerButton->raise();
        registerNotImplementedLabel->raise();
        authFailedLabel->raise();
        passwordTextbox->raise();
        loadingLabel->raise();
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
#if QT_CONFIG(tooltip)
        label_3->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        label_3->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        label_3->setText(QCoreApplication::translate("Login", "Don't have an account? Register now!", nullptr));
        loginButton->setText(QCoreApplication::translate("Login", "Login", nullptr));
        registerButton->setText(QCoreApplication::translate("Login", "Register", nullptr));
        registerNotImplementedLabel->setText(QString());
        authFailedLabel->setText(QString());
        passwordTextbox->setInputMask(QString());
        passwordTextbox->setPlaceholderText(QCoreApplication::translate("Login", "Password", nullptr));
        usernameTextbox->setPlaceholderText(QCoreApplication::translate("Login", "Username", nullptr));
        loadingLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
