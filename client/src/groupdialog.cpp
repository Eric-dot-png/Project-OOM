#include "groupdialog.h"
#include "ui_groupdialog.h"
#include <QCheckBox>
#include <QDebug>

groupDialog::groupDialog(OOMWidget* widget, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::groupDialog)
    , oomWidget(widget)
{
    ui->setupUi(this);



    connect(ui->createButton, &QPushButton::clicked, this, &groupDialog::getGroupMembers);
    connect(ui->closeButton, &QPushButton::clicked, this, [=](){
        // reset form for next use
        while (QLayoutItem* item = ui->checkBoxLayout->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        ui->groupNameTextbox->clear();
        ui->errorLabel->clear();
        this->close();
    });
}

void groupDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    for (const QString &frnd : oomWidget->getClient()->getFriendList().friends()) {
        qDebug() << "Adding friend: " << frnd;
        QCheckBox *cb = new QCheckBox(frnd, this);
        ui->checkBoxLayout->addWidget(cb);
    }

}

void groupDialog::getGroupMembers()
{


    QString t = "";
    if (ui->groupNameTextbox->text().isEmpty())
    {
        t = ui->errorLabel->text();
        ui->errorLabel->setText("Group must have a name!");
        return;
    }

    for (int i = 0; i < ui->checkBoxLayout->count(); ++i)
    {
        QWidget *w = ui->checkBoxLayout->itemAt(i)->widget();
        QCheckBox *cb = qobject_cast<QCheckBox*>(w);

        if (cb && cb->isChecked())
        {
            qDebug() << "Checked: " << cb->text();
            groupMembers.append(cb->text());
        }
    }

    QString name = ui->groupNameTextbox->text();
    emit groupMembersSelected(name, groupMembers);


    // reset form for next use
    while (QLayoutItem* item = ui->checkBoxLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    ui->groupNameTextbox->clear();
    ui->errorLabel->setText(t);
    this->close();
}

groupDialog::~groupDialog()
{
    delete ui;
}


