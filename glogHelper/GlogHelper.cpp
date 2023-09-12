//
// Created by lining on 2023/2/20.
//

#include "GlogHelper.h"
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>

GlogHelper::GlogHelper(std::string _program, uint _keep, std::string _logDir, bool _isSendSTDOUT) :
        program(_program), keepDays(_keep), logDir(_logDir), isSendSTDOUT(_isSendSTDOUT) {
    google::InitGoogleLogging(program.data());
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&FatalMessageDump);
    FLAGS_log_dir = logDir;
    FLAGS_logbufsecs = 0;//刷新日志buffer的时间，0就是立即刷新
    FLAGS_stop_logging_if_full_disk = true; //设置是否在磁盘已满时避免日志记录到磁盘
    if (isSendSTDOUT) {
        FLAGS_logtostdout = true;
    }
    isRun = true;
    futureRun = std::async(std::launch::async, cleaner, this);
}

GlogHelper::~GlogHelper() {
    isRun = false;
    try {
        futureRun.wait();
    } catch (std::future_error e) {
        LOG(ERROR) << e.what();
    }
    google::ShutdownGoogleLogging();
}

static void GetDirFiles(std::string path, std::vector<std::string> &array) {
    DIR *dir;
    struct dirent *ptr;
    char base[1024];
    if ((dir = opendir(path.c_str())) == nullptr) {
        printf("can not open dir %s\n", path.c_str());
        return;
    } else {
        while ((ptr = readdir(dir)) != nullptr) {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0)) {
//            printf("it is dir\n");
            } else if (ptr->d_type == DT_REG) {
                std::string name = ptr->d_name;
                array.push_back(name);
            }
        }
        closedir(dir);
    }
}

static bool startsWith(const std::string str, const std::string prefix) {
    return (str.rfind(prefix, 0) == 0);
}

static bool endsWith(const std::string str, const std::string suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }

    return (str.rfind(suffix) == (str.length() - suffix.length()));
}

int GlogHelper::cleaner(void *p) {
    if (p == nullptr) {
        return -1;
    }
    GlogHelper *local = (GlogHelper *) p;
    uint64_t keepSeconds = local->keepDays * 60 * 60 * 24;
    LOG(WARNING) << "开启日志定时清理任务";
    while (local->isRun) {
        sleep(5);
        //获取符合条件的日志文件 {ProgramName-*.log}
        std::vector<std::string> files;
        files.clear();
        GetDirFiles(local->logDir, files);
        std::vector<std::string> logs;
        logs.clear();
        for (auto &iter: files) {
            if (startsWith(iter, local->program + ".")) {
                logs.push_back(iter);
            }
        }
        if (!logs.empty()) {
            //比较文件的创建日志和现在的时间
            time_t now;
            time(&now);
            struct stat buf;
            for (auto &iter: logs) {
                std::string fulPath = local->logDir + "/" + iter;
                if (stat(fulPath.c_str(), &buf) == 0) {
                    if ((now - buf.st_ctime) > keepSeconds) {
                        LOG(INFO) << "准备删除文件 " << fulPath;
                        if (remove(fulPath.c_str()) == 0) {
                            char bufInfo[512];
                            memset(bufInfo, 0, 512);
                            sprintf(bufInfo, "log file:%s create time:%s,now:%s,keepSeconds:%d s,delete",
                                    fulPath.c_str(), asctime(localtime(&buf.st_ctime)), asctime(localtime(&now)),
                                    keepSeconds);
                            LOG(INFO) << bufInfo;
                        }
                    }
                }
            }
        }
    }
    LOG(WARNING) << "关闭日志定时清理任务";
    return 0;
}

void GlogHelper::FatalMessageDump(const char *data, unsigned long size) {
    std::ofstream fs("./fatal.log", std::ios::app);
    std::string str = std::string(data, size);
    fs << str;
    fs.close();
    LOG(FATAL) << str;
}
