#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include "oomwidget.h"
#include <QDialog>
#include <QStringList>
#include <QShowEvent>
#include <unordered_set>

namespace Ui {
class groupDialog;
}

class groupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit groupDialog(OOMWidget* widget, QWidget *parent = nullptr);
    ~groupDialog();

    void getGroupMembers();

signals:
    void groupMembersSelected(const QString& groupName, const QStringList& members);

private:
    void showEvent(QShowEvent *event);

    Ui::groupDialog *ui;
    QStringList groupMembers;
    OOMWidget* oomWidget;
};

#endif // GROUPDIALOG_H
