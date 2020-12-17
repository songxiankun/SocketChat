#include "chatroom.h"
#include "userlogin.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserLogin userLogin;
    userLogin.show();

    return a.exec();
}
