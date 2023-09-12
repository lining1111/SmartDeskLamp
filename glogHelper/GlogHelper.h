//
// Created by lining on 2023/2/20.
//

#ifndef GLOGHELPER_H
#define GLOGHELPER_H
#include <string>
#include <future>

class GlogHelper {
private:
    bool isRun = false;
    std::future<int> futureRun;

    uint scanPeriod = 5;//5s扫描时间
    uint keepDays = 1;//日志保持时间
    std::string program;
    std::string logDir;
    bool isSendSTDOUT;
public:
    GlogHelper(std::string _program, uint _keep, std::string _logDir, bool _isSendSTDOUT);
    ~GlogHelper();

private:
    static int cleaner(void *p);

    static void FatalMessageDump(const char *data, unsigned long size);

};


#endif //GLOGHELPER_H
