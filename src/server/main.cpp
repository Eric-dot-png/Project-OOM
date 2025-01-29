#include <iostream>
#include <QtCore>
#include <QtSql>

#include "dbHandler.h"

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("DSDB");
    db.setUserName("root"); //Change to your credentials
    db.setPassword("root"); //Change to your credentials
    db.open();
    
    dbHandler x;
    Person jdoe("jdoe", "abc", "jdoe@gmail.com");
    bool valid = x.availUsername(jdoe);
    if(valid)
        std::cout << x.newUser(jdoe) << std::endl;
    else
        std::cout << "Already Used!\n";

    db.close();
    return app.exec();
}

