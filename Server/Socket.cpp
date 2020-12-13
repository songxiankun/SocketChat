//
// Created by 宋贤坤 on 2020/12/12.
//

#include <sstream>
#include <utility>
#include "Socket.hpp"

Socket::Socket() {
    logs = new RecordLogs("log/log.txt", std::ios::app);
    memset(&serverAddr, 0, sizeof serverAddr);
    // 初始化服务器地址和端口
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // 初始化socket
    listener = 0;
#if __APPLE__
    kqufd = 0;
#elif __linux__
    // epool fd
    epfd = 0;
#endif
}

Socket::~Socket() {
    delete logs;
}

void Socket::Init() {

    // Step 1：创建监听socket
    // 使用socket()
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CheckFd(listener, getCurLineAndFileName(__LINE__, __FILE_NAME__) + " : socket 初始化错误!!! ");
    // Step 2：绑定地址
    // 使用bind()
    CheckFd(bind(listener, (struct sockaddr*)&serverAddr, sizeof serverAddr),
            getCurLineAndFileName(__LINE__, __FILE_NAME__) + " : bind()失败!!!");
    // Step 3：监听连接
    // 使用listen()
    CheckFd(listen(listener, 5),
            getCurLineAndFileName(__LINE__, __FILE_NAME__) + " : listen 失败!!!");

#if __APPLE__
    kqufd = kqueue();
    CheckFd(kqufd,
    getCurLineAndFileName(__LINE__, __FILE_NAME__) + " : kqueue 失败!!!");
    /* init kevent structure */
    KQueueAddFd(kqufd, listener);
#elif __linux__
    // Step 4：创建事件表
    // epoll_create()
    epfd = epoll_create(EPOLL_SIZE);
    CheckFd(epfd, getCurLineAndFileName(__LINE__, __FILE_NAME__) + " : epoll_create 失败!!!");
    // Step 5：添加监听fd到epoll fd
     //往事件表里添加监听事件
    EpollAddFd(epfd, listener, true);
#endif

}

void Socket::Close() {
    // close
    close(listener);

#if __APPLE__
    close(kqufd);
#elif __linux__
    close(epfd);
#endif
}

int Socket::SendBroadcastMessage(int connfd) {
    char buff[BUF_SIZE]{}, message[BUF_SIZE]{};
    memset(buff, 0, BUF_SIZE);
    memset(message, 0, BUF_SIZE);
    // Step 1：接收新消息 // recv()
    std::cout << "read from client(clientID = " << connfd << ")" << std::endl;
    int len = recv(connfd, buff, BUF_SIZE, 0);
    // Step 2：判断是否是客户端中止连接
    if (len == 0)
    {
        // 关闭链接文件描述符
        close(connfd);

        // 在客户端列表中删除该客户端
        clients_list.remove(connfd);
        std::cout << "ClientID = " << connfd
             << " closed.\n now there are "
             << clients_list.size()
             << " client in the char room"
             << std::endl;
    }
        // 发送广播消息给所有客户端
    else
    {
        // 判断是否聊天室还有其他客户端
        if(clients_list.size() == 1) {
            // 发送提示消息
            send(connfd, CAUTION, strlen(CAUTION), 0);
            return len;
        }
        // 格式化发送的消息内容
        sprintf(message, SERVER_MESSAGE, connfd, buff);

        // 遍历客户端列表依次发送消息，需要判断不要给来源客户端发
        std::list<int>::iterator it;
        for(it = clients_list.begin(); it != clients_list.end(); ++it) {
            if(*it != connfd){
                if( send(*it, message, BUF_SIZE, 0) < 0 ) {
                    return -1;
                }
            }
        }
    }
    return len;
}

void Socket::Start() {
    // Step 2：进入主循环

    // Step 3：获取就绪的事件
    // epoll_wait()

    // Step 4：循环处理所有就绪的事件
    // 4.1 如果是新连接则接受连接并将连接添加到epoll fd
    // accept() addfd()

    // 4.2 如果是新消息则广播给其他客户端
    // SendBroadcastMessage
}

/**
 * @description     检测socket的每一步返回值
 * @param fd        socket步骤的返回值
 * @param msg       错误提示
 */
void Socket::CheckFd(const int &fd, std::string msg) {
    if (fd < 0)
        logs->writeLogs(std::move(msg));
}

/**
 * @description     当前行和当前文件名字符串拼接
 * @param line      行
 * @param filename  文件名
 * @return  string  处理好后的字符串
 */
std::string Socket::getCurLineAndFileName(const int &line,
                                          const std::string &filename) {
    std::stringstream ss;
    ss << line;
    return std::string("filename :" + filename + "  line:" + ss.str());
}
