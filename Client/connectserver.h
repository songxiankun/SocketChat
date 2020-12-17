#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H

#include <QDialog>
#include <QMessageBox>
#include "Common.hpp"


class ConnectServer : public QDialog
{
    Q_OBJECT
public:
    ConnectServer(QWidget *parent = nullptr);
    virtual ~ConnectServer();

    void Connect();

    const SOCKET& GetConnFd();

    // send
    int Send(const SOCKET& fd, void* buff, const size_t& buff_len);
    // recv
    int Recv(const SOCKET& fd, void* &buff, const size_t& buff_len);
    // 关闭
    void Close(SOCKET& fd);

private:
    SOCKET connfd;
    // 服务端serveraddr结构
    struct sockaddr_in serverAddr{};
    // 服务端地址结构信息长度
    int serverLen;
    // 初始化socket
    void Socket(const int& family, const int& tyep, const int& protocol);
    // connect server
    void Connect(const SOCKET& fd, void* server, const size_t& len);
    // 检测
    bool IsValid(const SOCKET &result);
    // 错误处理
    void DealError(bool isvalid);

};


#endif // CONNECTSERVER_H
