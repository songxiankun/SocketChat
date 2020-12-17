//
// Created by 宋贤坤 on 2020/12/16.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include "Common.hpp"
#include "FileOperator.hpp"


class Server {

private:
    SOCKET _socket;                     // socket初始化文件描述符
    struct sockaddr_in _server{};       // 服务端Socket address 数据结构
    struct sockaddr_in _client{};       // 客户端Socket address 结构体
    unsigned int client_len;            // 客户端Socket address大小
    FileOperator* doFile;               // 文件操作指针
    fd_set rfds{};                        // 读集合
    fd_set wfds{};                        // 写集合
    int maxfd;                          // 当前文件描述符+1
    bool isChange;                      // 当前是否有新的用户加入或者有用户进行数据处理

    void Socket();
    void Bind();
    void Listen();
    void Accept();
    // select 模型
    void Select();
    // 是否更新select集合 优化
    void Update(bool);
    // 发送
    int Send(const SOCKET& fd, const void* buff, const int& bufflen);
    // 接收
    int Recv(const SOCKET& fd, void* buff, const int& bufflen);
    // 发送
    void SendMsg(const SOCKET& fd, const void* buff, const int& bufflen, const int& type);
    // 收取
    void RecvMsg(const SOCKET& fd);
    // 广播发送消息
    void BroadcaseMsg(const SOCKET& fd, const std::string& msg, bool isBroad = true);
    // 数据解析
    bool Login(const SOCKET& fd, std::string& login_msg, const int& already_len);
    // 关闭
    void Close(SOCKET& fd);
    // 多进程
    static bool IsValid(const SOCKET& result);
public:
    Server();
    ~Server();
    void ServerRun();


};


#endif //SERVER_SERVER_HPP
