#include <iostream>
#include "opencv2/core/version.hpp"
#include <dirent.h>
#include "glogHelper/GlogHelper.h"
#include <unistd.h>
#include "version.h"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "os/os.h"
#include "config/config.h"
#include <string>
#include <csignal>

using namespace std;

int signalIgnPipe() {
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    sigemptyset(&act.sa_mask);
    act.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &act, NULL) < 0) {
        printf("call sigaction fail, %s\n", strerror(errno));
        return errno;
    }

    return 0;
}

DEFINE_int32(keep, 5, "日志清理周期 单位day，默认5");
DEFINE_bool(isSendSTDOUT, false, "输出到控制台，默认false");
DEFINE_string(logDir, "log", "日志的输出目录,默认log");
int main(int argc, char **argv) {
    signalIgnPipe();

    gflags::SetVersionString(VERSION_BUILD_TIME);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string proFul = std::string(argv[0]);
    std::string pro = os::g_getFileName(proFul);

    //日志系统类
    GlogHelper glogHelper(pro, FLAGS_keep, FLAGS_logDir, FLAGS_isSendSTDOUT);
    LOG(WARNING) << "程序工作目录:" << string(get_current_dir_name()) << ",版本号:" << VERSION_BUILD_TIME;


    std::cout << "Hello, World!" << CV_VERSION <<std::endl;

    return 0;
}
