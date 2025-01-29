#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Profile; }
QT_END_NAMESPACE

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
