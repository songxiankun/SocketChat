//
// Created by 宋贤坤 on 2020/12/12.
//

#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "Config.hpp"
#include "RecordLogs.hpp"
#include <list>

class Socket
{
public:
    // 无参数构造函数
    Socket();
    // 析构函数
    ~Socket();
    // 初始化服务器端设置
    void Init();
    // 关闭服务
    void Close();
    // 启动服务端
    void Start();

private:
    // 广播消息给所有客户端
    int SendBroadcastMessage(int clientfd);
    // 服务器端serverAddr信息
    struct sockaddr_in serverAddr{};
    //创建监听的socket
    int listener{};
    // 简单记录日志
    RecordLogs* logs;

    // 检测是否处理成功
    void CheckFd(const int& fd, std::string msg);

    static std::string getCurLineAndFileName(const int& line = __LINE__,
                                             const std::string& filename = __FILE_NAME__);


#if __APPLE__
    int kqufd;
#elif __linux__
    // epoll_create创建后的返回值
    int epfd;
#endif
    // 客户端列表
    std::list<int> clients_list;
};


#endif //SERVER_SOCKET_HPP
