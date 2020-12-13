//
// Created by 宋贤坤 on 2020/12/12.
//

#include "RecordLogs.hpp"

RecordLogs::RecordLogs(const char *file, const unsigned& mod) : BaseLogs(file, mod) {
    out.open(file, mod);
    out.seekp(0L, std::ios::beg);
}

RecordLogs::~RecordLogs() {
    std::cout << "~RecordLogs()" << std::endl;
    out.close();
}

void RecordLogs::writeLogs(std::string msg) {
    out << msg << std::endl;
}
