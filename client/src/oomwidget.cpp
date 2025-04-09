#include "oomwidget.h"
#include "applicationhandler.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QShortcut>



Client* OOMWidget::client = nullptr;



void OOMWidget::startApp()
{
    QShortcut *reloadShortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(reloadShortcut, &QShortcut::activated, this, &OOMWidget::reloadStyleSheet);

    //Main loop
    ApplicationHandler *app = new ApplicationHandler();

    auto const rec = QApplication::desktop()->availableGeometry(this);
    auto const height = rec.height();
    auto const width = rec.width();
    qDebug() << "h: " << rec.height() << ' ' << "w: " << rec.width();
    app->resize(height, width);
    app->show();
}

QString OOMWidget::loadStyleSheet(const QString &resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to load stylesheet:" << resourcePath;
        return "";
    }
    return QString::fromUtf8(file.readAll());
}

void OOMWidget::reloadStyleSheet() {
    QFile file(":/stylesheets/stylesheets/lightmode.qss");
    qDebug() << "Hotreloading stylesheet.";
    if (file.open(QFile::ReadOnly)) {
        QString style = file.readAll();
        qApp->setStyleSheet(style);
    }
}
