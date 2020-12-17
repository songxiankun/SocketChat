#include "connectserver.h"
#include <QMessageBox>

ConnectServer::ConnectServer(QWidget *parent) :
    QDialog(parent), connfd(-1), serverLen(0)
{
    serverLen = sizeof (serverAddr);
    memset(&serverAddr, 0, serverLen);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);
}

ConnectServer::~ConnectServer()
{
    Close(connfd);
}


void ConnectServer::Connect()
{
    Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Connect(connfd, (void*)&serverAddr, serverLen);
}

const SOCKET& ConnectServer::GetConnFd()
{
    return this->connfd;
}


// 初始化socket
void ConnectServer::Socket(const int& family, const int& type, const int& protocol)
{
    connfd = socket(family, type, protocol);
    // 函数检测
    DealError(IsValid(connfd));
}

// connect server
void ConnectServer::Connect(const SOCKET& fd, void* server, const size_t& len)
{
    DealError(
                IsValid(
                    ::connect(fd, (struct sockaddr*)server, len)
                    )
                );
}

// send
int ConnectServer::Send(const SOCKET& fd, void* buff, const size_t& buff_len)
{
    return send(fd, buff, buff_len, 0);
}

// recv
int ConnectServer::Recv(const SOCKET& fd, void* &buff, const size_t& buff_len)
{
    return recv(fd, buff, buff_len, 0);
}

// 关闭
void ConnectServer::Close(SOCKET& fd)
{
    if (IsValid(fd))
    {
        ::close(fd);
        fd = -1;
    }
}

// 错误检测
bool ConnectServer::IsValid(const SOCKET &result) {
    return result != INVALID_SOCKET;
}

// 错误处理
void ConnectServer::DealError(bool isvalid)
{
    if (!isvalid)
    {
        // TODO 记录日志
        QMessageBox::critical(this, "socket错误", "socket服务连接失败，请联系管理员!!!");
        exit(1);
    }
}
