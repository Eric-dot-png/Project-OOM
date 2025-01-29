/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *registerNotImplementedLabel;
    QLabel *authFailedLabel;
    QLineEdit *passwordTextbox;
    QLineEdit *usernameTextbox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(800, 600);
        centralwidget = new QWidget(Login);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 30, 81, 20));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 80, 91, 20));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 130, 271, 20));
        loginButton = new QPushButton(centralwidget);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(260, 80, 83, 29));
        registerButton = new QPushButton(centralwidget);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(100, 160, 83, 29));
        registerNotImplementedLabel = new QLabel(centralwidget);
        registerNotImplementedLabel->setObjectName("registerNotImplementedLabel");
        registerNotImplementedLabel->setEnabled(false);
        registerNotImplementedLabel->setGeometry(QRect(70, 200, 151, 20));
        authFailedLabel = new QLabel(centralwidget);
        authFailedLabel->setObjectName("authFailedLabel");
        authFailedLabel->setEnabled(false);
        authFailedLabel->setGeometry(QRect(250, 30, 231, 41));
        authFailedLabel->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        authFailedLabel->setWordWrap(true);
        passwordTextbox = new QLineEdit(centralwidget);
        passwordTextbox->setObjectName("passwordTextbox");
        passwordTextbox->setGeometry(QRect(110, 80, 113, 28));
        passwordTextbox->setEchoMode(QLineEdit::EchoMode::Password);
        usernameTextbox = new QLineEdit(centralwidget);
        usernameTextbox->setObjectName("usernameTextbox");
        usernameTextbox->setGeometry(QRect(110, 30, 113, 28));
        usernameTextbox->setClearButtonEnabled(false);
        Login->setCentralWidget(centralwidget);
        usernameTextbox->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        loginButton->raise();
        registerButton->raise();
        registerNotImplementedLabel->raise();
        authFailedLabel->raise();
        passwordTextbox->raise();
        menubar = new QMenuBar(Login);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        Login->setMenuBar(menubar);
        statusbar = new QStatusBar(Login);
        statusbar->setObjectName("statusbar");
        Login->setStatusBar(statusbar);

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QMainWindow *Login)
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
