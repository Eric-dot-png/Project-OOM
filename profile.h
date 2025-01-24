#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>

namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr, QString *username = new QString("Default"));
    ~Profile();

private:
    Ui::Profile *ui;
    QString *username;
};

#endif // PROFILE_H
