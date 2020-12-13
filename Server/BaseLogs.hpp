//
// Created by 宋贤坤 on 2020/12/12.
//

#ifndef SERVER_BASELOGS_HPP
#define SERVER_BASELOGS_HPP

#include <iostream>
#include <cstring>

class BaseLogs {
public:
    BaseLogs(const char* file, const unsigned mod){}
    virtual ~BaseLogs() = default;
    virtual void writeLogs(std::string msg) = 0;

};


#endif //SERVER_BASELOGS_HPP
