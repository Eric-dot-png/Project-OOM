#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_backButton_clicked();

    void on_registerButton_clicked();

    void on_emailEdit_returnPressed();

    void on_passwordEdit_returnPressed();

    void on_usrnameEdit_returnPressed();

    void on_registerButton_pressed();

signals:
    void showLogin();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
