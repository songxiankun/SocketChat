//
// Created by 宋贤坤 on 2020/12/12.
//

#include "RecordLogs.hpp"
#include <sstream>

std::string getCurLineAndFileName(const int &line = __LINE__,
                                  const std::string &filename = __FILE_NAME__) {
    std::stringstream ss;
    ss << line;
    return std::string(filename + "  " + ss.str());
}

int main()
{
    RecordLogs logs("log.txt", std::ios::in);
    for (int i = 0; i < 10; ++i) {
        logs.writeLogs(std::move(getCurLineAndFileName(__LINE__, __FILE_NAME__) + " 2 "));
    }
    return 0;
}
