#include "oomwidget.h"
#include "applicationhandler.h"

Client* OOMWidget::client = nullptr;

void OOMWidget::startApp()
{
    //Main loop
    ApplicationHandler *app = new ApplicationHandler();
    app->resize(800, 600);
    app->show();
}
