/********************************************************************************
** Form generated from reading UI file 'authenticationcode.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHENTICATIONCODE_H
#define UI_AUTHENTICATIONCODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_authenticationCode
{
public:
    QLineEdit *codeTextbox;
    QLabel *label;
    QPushButton *submitButton;
    QLabel *authFailedLabel;
    QLabel *loadingLabel;

    void setupUi(QWidget *authenticationCode)
    {
        if (authenticationCode->objectName().isEmpty())
            authenticationCode->setObjectName(QString::fromUtf8("authenticationCode"));
        authenticationCode->resize(640, 480);
        codeTextbox = new QLineEdit(authenticationCode);
        codeTextbox->setObjectName(QString::fromUtf8("codeTextbox"));
        codeTextbox->setGeometry(QRect(130, 160, 131, 26));
        label = new QLabel(authenticationCode);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(130, 120, 141, 31));
        submitButton = new QPushButton(authenticationCode);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setGeometry(QRect(270, 155, 91, 31));
        submitButton->setCursor(QCursor(Qt::PointingHandCursor));
        authFailedLabel = new QLabel(authenticationCode);
        authFailedLabel->setObjectName(QString::fromUtf8("authFailedLabel"));
        authFailedLabel->setGeometry(QRect(130, 190, 161, 71));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        authFailedLabel->setFont(font);
        authFailedLabel->setWordWrap(true);
        loadingLabel = new QLabel(authenticationCode);
        loadingLabel->setObjectName(QString::fromUtf8("loadingLabel"));
        loadingLabel->setGeometry(QRect(270, 200, 45, 45));

        retranslateUi(authenticationCode);

        QMetaObject::connectSlotsByName(authenticationCode);
    } // setupUi

    void retranslateUi(QWidget *authenticationCode)
    {
        authenticationCode->setWindowTitle(QCoreApplication::translate("authenticationCode", "Form", nullptr));
        label->setText(QCoreApplication::translate("authenticationCode", "Authentication code:", nullptr));
        submitButton->setText(QCoreApplication::translate("authenticationCode", "Submit", nullptr));
        authFailedLabel->setText(QString());
        loadingLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class authenticationCode: public Ui_authenticationCode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHENTICATIONCODE_H
