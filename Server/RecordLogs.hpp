//
// Created by 宋贤坤 on 2020/12/12.
//

#ifndef SERVER_RECORD_HPP
#define SERVER_RECORD_HPP

#include "BaseLogs.hpp"
#include <iostream>
#include <fstream>

class RecordLogs : public BaseLogs
{
private:
    std::ofstream out;

public:
    RecordLogs(const char *file, const unsigned& mod);
    ~RecordLogs() override;

    void writeLogs(std::string msg) override;

};


#endif //SERVER_RECORD_HPP
