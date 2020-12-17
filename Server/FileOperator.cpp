//
// Created by 宋贤坤 on 2020/12/16.
//

#include "FileOperator.hpp"

FileOperator::FileOperator() : open(nullptr) {
    open = new std::fstream;
    open->open("userdata.dat", std::ios::out);
}

FileOperator::~FileOperator() {
    if (nullptr != open) {
        open->close();
        delete open;
        open = nullptr;
    }
}

bool FileOperator::UserLogin(const std::string &username, const std::string &password, std::string &msg) {
    // 参数检测
    if (username.empty() || password.empty()) {
        msg = "error: 用户名或密码不能为空!!!";
        return false;
    }

    // 文件是否打开
    if (!open->is_open()) {
        msg = "error: 用户文件遗失，请联系管理员!!!";
        return false;
    }

    // 文件读取文件
    std::string file_user;
    std::string file_pass;

    while (!open->eof()) // 是否到达文件末尾
    {
        *open >> file_user >> file_pass;

        if (file_user == username && file_pass == password) {
            msg = "success: 登陆成功!!!";
            // 关闭文件指针
            open->close();
            open = nullptr;
            return true;
        }
    }
    msg = "error: 账号或用户名错误!!!";
    // 关闭文件指针
    open->close();
    open = nullptr;
    return false;
}