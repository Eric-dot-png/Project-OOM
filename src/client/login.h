#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

enum class Client {
    LoggedOut,
    LoggingIn,
    LoggedIn
};

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

    constexpr bool getStatus(Client currentStatus, Client otherStatus);

    void update();

signals:


private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();

private:
    Client currentStatus;
    Ui::Login *ui;
    bool authenticated;

};
#endif // LOGIN_H
