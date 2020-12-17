//
// Created by 宋贤坤 on 2020/12/16.
//

#ifndef SERVER_FILEOPERATOR_HPP
#define SERVER_FILEOPERATOR_HPP

#include <iostream>
#include <fstream>


class FileOperator {
private:
    std::fstream* open;

public:
    FileOperator();
    ~FileOperator();

    bool UserLogin(const std::string& username, const std::string& password, std::string& msg);


};


#endif //SERVER_FILEOPERATOR_HPP
