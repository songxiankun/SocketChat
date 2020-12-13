//
// Created by 宋贤坤 on 2020/12/12.
//

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#if __APPLE__       /* mac os use kqueue*/
#include <sys/event.h>
#elif __linux__
#include <sys/epoll.h>  /* linux use epoll */
#endif

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// 默认服务器端IP地址
#define SERVER_IP "127.0.0.1"       // 配置IP INADDR_ANY

// 服务器端口号
#define SERVER_PORT 52100            // use 49152-65535

#if __APPLE__   /* macos */
#define CONCURRENT_MAX 8 //应用层同时可以处理的连接
//类似epoll_create
int client_fds[CONCURRENT_MAX];
struct kevent events[10];       //CONCURRENT_MAX + 2
#elif __linux__
// int epoll_create(int size)中的size
// 为epoll支持的最大句柄数
#define EPOLL_SIZE 5000
#endif
// 缓冲区大小65535
#define BUF_SIZE 0xFFFF

// 新用户登录后的欢迎信息
#define SERVER_WELCOME "Welcome you join to the chat room! Your chat ID is: Client #%d"

// 其他用户收到消息的前缀
#define SERVER_MESSAGE "ClientID %d say >> %s"

// 退出系统
#define EXIT "EXIT"

// 提醒你是聊天室中唯一的客户
#define CAUTION "There is only one int the char room!"


#if __APPLE__

static void KQueueAddFd(int kq, int fd) {
    // 定时任务
    struct timespec timeout = {10,0};
    // 创建kqueue
    // 创建struct kevent变量（注意这里的kevent是结构体类型名），可以通过EV_SET这个宏提供的快捷方式进行创建
    // 通过kevent系统调用将创建好的kevent结构体变量加入到kqueue队列中，完成对指定文件描述符的事件的订阅
    // 通过kevent系统调用获取满足条件的事件队列，并对每一个事件进行处理
    struct kevent event_change{};
    EV_SET(&event_change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    kevent(kq, &event_change, 1, nullptr, 0, nullptr);
    EV_SET(&event_change, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event_change, 1, nullptr, 0, nullptr);

}

#elif __linux__
// 注册新的fd到epollfd中
// 参数enable_et表示是否启用ET模式，如果为True则启用，否则使用LT模式
static void EpollAddFd( int epollfd, int fd, bool enable_et )
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if( enable_et )
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    // 设置socket为nonblocking模式
    // 执行完就转向下一条指令，不管函数有没有返回。
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)| O_NONBLOCK);
    std::cout << "fd added to epoll!" << std::endl;
}
#endif

#endif //SERVER_CONFIG_HPP
