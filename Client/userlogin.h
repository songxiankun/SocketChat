#ifndef USERLOGIN_H
#define USERLOGIN_H

#include "connectserver.h"
#include <string.h>
#include "chatroom.h"

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class UserLogin;
}

class UserLogin : public QDialog
{
    Q_OBJECT

public:
    explicit UserLogin(QWidget *parent = nullptr);
    virtual ~UserLogin();

private slots:
    void on__login_clicked();

private:
    ConnectServer conn;             // 长连接
    Ui::UserLogin *ui;
    ChatRoom* chat;                 // 聊天程序
};

#endif // USERLOGIN_H
