//
// Created by 宋贤坤 on 2020/12/16.
// socket server and client common configuration
//

#ifndef SERVER_COMMON_HPP
#define SERVER_COMMON_HPP

#include <QString>

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
//#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>

// 消息传递信息封装----结构化消息 //
enum CMD
{
    CMD_LOGIN,
    CMD_LOGOUT,
    CMD_WECHAT,
    CMD_LOGINOUT_RESULT
};

// 报文头部
struct DataHeader
{
    explicit DataHeader(short CMD_) : cmd(CMD_) {}
    short  dataLength{};
    short  cmd;
};

// 登陆信息
struct Login : public DataHeader
{
    Login() : DataHeader(CMD_LOGIN)
    {
        dataLength = sizeof(Login);
    }
    char userName[32]{};
    char passWord[32]{};
};

// 登出
struct Logout : public DataHeader
{
    Logout() : DataHeader(CMD_LOGOUT)
    {
        dataLength = sizeof(Logout);
    }
};

// 结果消息返回信息
struct LogInOutResult : public DataHeader
{
    explicit LogInOutResult(short CMD_) : DataHeader(CMD_)
    {
        dataLength = sizeof(LogInOutResult);
    }
    int             result{};
    char            loginMsg[128];
};
//  消息提封装结束 //

// alias int to SOCKET //
typedef int SOCKET;
typedef struct Login* pLOGIN;
typedef struct LogInOutResult* pLIOR;

// 错误的返回值-1
const int INVALID_SOCKET = -1;


// 默认服务器IP地址
#define SERVER_IP "127.0.0.1"
// 服务器端口号
#define SERVER_PORT 52100
// 监听数量
//const int BACK_LOG = 5;
// 最大连接数
//const int MAX_SIZE = 100;
// 连接客户端的用户 存入list
//std::list<int> client_list;
#endif //SERVER_COMMON_HPP
