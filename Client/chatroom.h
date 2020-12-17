#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatRoom; }
QT_END_NAMESPACE

class ChatRoom : public QWidget
{
    Q_OBJECT

public:
    ChatRoom(QWidget *parent = nullptr);
    ~ChatRoom();

private:
    Ui::ChatRoom *ui;
};
#endif // CHATROOM_H
