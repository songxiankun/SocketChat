#include "userlogin.h"
#include "ui_userlogin.h"

UserLogin::UserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);
    // 连接服务器
    conn.Connect();
}

UserLogin::~UserLogin()
{
    if (nullptr == chat)
        delete chat;
    delete ui;
}

// 登陆点击事件
void UserLogin::on__login_clicked()
{
    // 获取用户输入的信息
    QString username = ui->_username->text().trimmed();
    QString password = ui->_password->text().trimmed();

    // 检测
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "错误", "用户或密码不能为空");
        return;
    }

    struct Login login;
    // 数据拷贝
    strcpy(login.userName, username.toLocal8Bit().data());
    strcpy(login.passWord, password.toLocal8Bit().data());

    // 发送
    int sendLen = conn.Send(conn.GetConnFd(), (void*)&login, login.dataLength);
    qDebug() << "已发送：" << sendLen << "字节. 发送失败：" << login.dataLength - sendLen << "字节." << endl;

    struct LogInOutResult log(-1);
//    void* _log = &log;

//    int recvLen = conn.Recv(conn.GetConnFd(), _log, log.dataLength);
    int recvLen = recv(conn.GetConnFd(), (void*)&log, log.dataLength, 0);
    qDebug() << "已接收：" << recvLen << "字节. 接收失败：" << log.dataLength - recvLen << "字节." << endl;

    qDebug() << "CMD : " << log.cmd << endl;
    qDebug() << " datalength: " << log.dataLength << endl;
    qDebug() << "log result: " << log.result << endl;
    qDebug() << "msg : " << log.loginMsg << "msg size: " << sizeof (log.loginMsg) << endl;

    if (log.result == 0)    // 登陆成功
    {
        QMessageBox::information(this, "登陆提示", log.loginMsg);
        chat = new ChatRoom;
        chat->show();
        this->close();
    }
    else if (log.result == 1)
    {
        QMessageBox::critical(this, "登陆提示", log.loginMsg);
        return;
    }
}
