#include "oomwidget.h"
#include "applicationhandler.h"

#include <QDesktopWidget>

Client* OOMWidget::client = nullptr;

void OOMWidget::startApp()
{
    //Main loop
    ApplicationHandler *app = new ApplicationHandler();

    auto const rec = QApplication::desktop()->availableGeometry(this);
    auto const height = rec.height();
    auto const width = rec.width();

    app->resize(height, width);
    app->show();
}
