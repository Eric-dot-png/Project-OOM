// file : main.cpp
// name : eric garcia

#include <QCoreApplication>

#include "client.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    oom::Client myclient(1234);
    
    return app.exec();
}
