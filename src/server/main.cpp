#include <iostream>
#include <QtCore>

#include "Server.h"
#include "dbHandler.h"

static dbHandler db;

int main()
{
    Person jdoe("jdoe", "abc", "jdoe@gmail.com");
    bool valid = x.availUsername(jdoe);
    if(valid)
        std::cout << x.newUser(jdoe) << std::endl;
    else
        std::cout << "Already Used!\n";

    return 0;
}

