#include "chatroom.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatRoom w;
    w.show();
    return a.exec();
}
