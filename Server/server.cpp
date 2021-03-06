//
// Created by 宋贤坤 on 2020/12/16.
//

#include "server.hpp"

Server::Server() : _socket(INVALID_SOCKET), client_len(sizeof _client),
                   doFile(nullptr), maxfd(0), isChange(true) {
    // 清除连接
    client_list.clear();
    // 置空
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);

    // 置0
    memset(&_server, 0, sizeof _server);
    memset(&_client, 0, sizeof _client);
    // 服务器初始化
    _server.sin_family = AF_INET;
    _server.sin_port = htons(SERVER_PORT);
    _server.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 初始化文件指针
    doFile = new FileOperator;
}

Server::~Server() {
    Close(_socket);
    // 关闭文件指针
    if (nullptr != doFile) {
        delete doFile;
        doFile = nullptr;
    }
}

/**
 * 初始化socket套接字描述符
 */
void Server::Socket() {
    int count = 0;
    FLAG:
    // 创建socket文件描述符
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 判断是否成功
    if (!IsValid(_socket) && count < 3) {
        count++;
        goto FLAG;
    } else if (count == 3)
        exit(1);
    // 重置计数器
    count = 0;
}

/**
 * bind 函数
 */
void Server::Bind() {
    int count = 0;
    FLAG:
    // 绑定socket
    int ret = bind(_socket, (struct sockaddr *) &_server, sizeof _server);

    // 判断是否成功绑定
    if (!IsValid(ret) && count < 3) {
        count++;
        goto FLAG;
    } else if (count == 3)  // 绑定失败
    {
        Close(_socket);
        exit(1);
    }
    // 重置计数器
    count = 0;
}

/**
 * 套接字监听
 */
void Server::Listen() {
    int count = 0;
    FLAG:
    int ret = listen(_socket, BACK_LOG);

    // 判断是否监听成功
    if (!IsValid(ret && count < 3)) {
        count++;
        goto FLAG;
    } else if (count == 3) {
        Close(_socket);
        exit(1);
    }
    count = 0;
}

/**
 * Accept 接收客户端连接
 */
void Server::Accept() {
    SOCKET connfd = -1;
    int count = 0;
    FLAG:
    connfd = accept(_socket, (struct sockaddr *) &_client, &client_len);

    std::clog << "connfd: " << connfd << "\t"
              << "client_ip : " << inet_ntoa(_client.sin_addr) << "\t"
              << "client_port: " << htons(_client.sin_port) << std::endl;

    // 判断
    if (!IsValid(connfd) && count < 3) {
        count++;
        goto FLAG;
    } else if (count == 3) {
        Close(_socket);
        count = 0;
        exit(1);
    } else {
        // 添加到list中
        new_client_list.push_back(connfd);
        //close(connfd);
        if (connfd > maxfd)
            maxfd = connfd;
    }
    count = 0;
}

// 简单发送接口
int Server::Send(const SOCKET &fd, const void *buff, const int &bufflen) {
    return send(fd, buff, (size_t) bufflen, 0);
}

// 封装发送
void Server::SendMsg(const SOCKET &fd, const void *buff, const int &bufflen, const int &type) {
    if (!IsValid(fd))
        return;
    int len = Send(fd, buff, bufflen);
    std::clog << "发送成功!!!, 一共发送：" << len << "字节！需要发送：" << bufflen << "字节!!!" << std::endl;
}

// 封装收取消息
void Server::RecvMsg(SOCKET &fd) {
    if (!IsValid(fd))
        return;

    // 先接收header
    struct DataHeader dataHeader(-1);
    // 清空数据
    memset(&dataHeader,0, sizeof dataHeader);
    // 获取接收的header大小
    short len = recv(fd, (char *) &dataHeader, sizeof(dataHeader), 0);
    // 测试
//    struct Login login;
//    recv(fd, (char*)&login + len, login.dataLength - len, 0);
//    std::cout << login.userName << " " << login.passWord << std::endl;
//    return;
//     测试 end
    // 如果接收消息<= 0 删除客户端
    if (len <= 0) {
        std::cout << "客户端退出" << std::endl;
        // 关闭连接
        // 客户端list 清除
        FD_CLR(fd, &rfds);
        FD_CLR(fd, &wfds);
        new_client_list.remove(fd);
        if (fd == maxfd)
            maxfd -= 1;
        close(fd);
        return;
    }


    // 接收消息msg
    std::string login_msg{};
    // 返回结果
    struct LogInOutResult logInOutResult(-1);
    // 发送命令
    switch (dataHeader.cmd) {
        case CMD_LOGIN:  // 登陆
        {
            if (Login(fd, login_msg, len)) {
                // 登陆成功
                memset(&logInOutResult, 0 , sizeof logInOutResult);
                strcpy(logInOutResult.loginMsg, login_msg.c_str());
                logInOutResult.result = 0;
                logInOutResult.cmd = CMD_LOGINOUT_RESULT;
                logInOutResult.dataLength = sizeof(logInOutResult);
                int send_len = Send(fd, (void *) &logInOutResult, logInOutResult.dataLength);
                std::clog << "登陆成功：发送数据长度为：" << send_len << "Bytes!!!" << std::endl;
            } else {
                // 登陆失败
                memset(&logInOutResult, 0 , sizeof logInOutResult);
                logInOutResult.cmd = CMD_LOGINOUT_RESULT;
                strcpy(logInOutResult.loginMsg, login_msg.c_str());
                logInOutResult.result = 1;
                logInOutResult.dataLength = sizeof(logInOutResult);
                int send_len = Send(fd, (void *) &logInOutResult, logInOutResult.dataLength);
                std::clog << "登陆失败：发送数据长度为：" << send_len << "Bytes!!!" << std::endl;
            }
            break;
        }
        case CMD_LOGOUT: {
            memset(&logInOutResult, 0 , sizeof logInOutResult);
            strcpy(logInOutResult.loginMsg, "success: 登出成功!!!");
            logInOutResult.result = 0;
            int send_len = Send(fd, (void *) &logInOutResult, logInOutResult.dataLength);
            std::clog << "登出成功：发送数据长度为：" << send_len << "Bytes!!!" << std::endl;
            break;
        }
        case CMD_WECHAT: { // TODO： 聊天处理

            break;
        }
        default:
            // TODO 其他处理
            break;
    }
}

/**
 * 发送广播｜私聊发送消息
 * @param fd       发送人fd
 * @param msg      消息
 * @param isBroad  默认群发 true
 */
void Server::BroadcaseMsg(const SOCKET &fd, const std::string &msg, bool isBroad) {

}

/**
 * 用户登陆
 * @param fd
 * @return
 */
bool Server::Login(SOCKET &fd, std::string &login_msg, const int &already_len) {
    if (!IsValid(fd))
        return false;

    // 登陆结构体
    struct Login login{};

    if (recv(fd, (char *) &login + already_len, login.dataLength - already_len, 0) <= 0) {
        login_msg = "error: 接收数据为空!!!";
        return false;
    }

    std::string login_user = login.userName;
    std::string login_pass = login.passWord;
    // 文件操作
    return doFile->UserLogin(login_user, login_pass, login_msg);
}

// 关闭
void Server::Close(SOCKET &fd) {
    if (IsValid(fd))  // 如果true
    {
        close(fd);
        fd = INVALID_SOCKET;
    }
}

/**
 * 返回值检测
 * @param result
 * @return true ｜ false
 */
bool Server::IsValid(const SOCKET &result) {
    return result != INVALID_SOCKET;
}

/**
 * 启动服务程序
 */
void Server::ServerRun() {
    Socket();
    Bind();
    Listen();
    Select();
}

/**
 * select 模型
 */
void Server::Select() {
    // 将创建的socket文件描述符加入list
    client_list.push_back(_socket);
    new_client_list.push_back(_socket);
    // 死循环
    for (;;) {
        // 遍历client_list是否有新加入的文件描述符将其写入到rfds 和 wdfs
        client_list.clear();
        client_list = new_client_list;
        for (auto iter : client_list) {
            if (iter > 0) {
                FD_SET(iter, &rfds);
                FD_SET(iter, &wfds);
                if (iter > maxfd)
                    maxfd = iter;
            }
        }
        int ret = select(maxfd + 1, &rfds, &wfds, nullptr, nullptr);
        switch (ret) {
            case 0: { // 超时返回0
                std::cerr << "error: timeout" << std::endl;
                break;
            }
            case -1: {  // 出错
                std::cerr << "error: select" << std::endl;
                exit(1);
                break;
            }
            default: {
                for (auto iter : client_list) {
                    if (iter == _socket && FD_ISSET(iter, &rfds)) {
                        //listen_sock happened read events
                        Accept();
                    } else if (iter != _socket && FD_ISSET(iter, &rfds)) {
                        // new_sock happened read events
                        // 获取消息
                        RecvMsg(iter);
                    } else if (iter != _socket && FD_ISSET(iter, &wfds)) {
                        // TODO 服务器发送消息给客户端
                    }
                }
                break;
            }
        }
    }
}


int main(int argc, char *argv[]) {
    Server server;
    server.ServerRun();
    return 0;
}