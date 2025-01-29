#ifndef BASEUSER_H
#define BASEUSER_H

#include <QObject>
#include <QString>

class BaseUser : public QObject
{
    Q_OBJECT
public:
    explicit BaseUser(QObject *parent = nullptr);
    BaseUser(int id, const QString &username, QObject *parent = nullptr);

    virtual ~BaseUser();

    int getId() const;
    QString getUsername() const;


signals:

private slots:


protected:
    int id;
    QString username;
};

#endif // BASEUSER_H
