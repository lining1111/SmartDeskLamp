//
// Created by lining on 9/30/22.
//

#ifndef _OS_H
#define _OS_H

#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include "iconv.h"

namespace os {
    using namespace std;

    int execute_command(const std::string &command, std::string *output = nullptr,
                        bool redirect_stderr = false);

    // 取文件夹名字 无后缀
    string g_getFolderPath(string str);

    // 取后缀
    string g_getFileSuffix(string str);

    // 取文件名字 不包括后缀
    string g_getFileName(string str);

    // 去掉后缀
    string g_getRemoveSuffix(string str);

    // 取文件名字 包括后缀
    string g_getFileNameAll(string str);

    int GetVectorFromFile(vector<uint8_t> &array, string filePath);

    int GetFileFromVector(vector<uint8_t> &array, string filePath);

    vector<string> split(const string &in, const string &delim);

    /**
     * 打印hex输出
     * @param data
     * @param len
     */
    void PrintHex(uint8_t *data, uint32_t len);

    class chs_codecvt : public codecvt_byname<wchar_t, char, std::mbstate_t> {
    public:
        chs_codecvt(string s) : codecvt_byname(s) {

        }
    };

    string Utf8ToGbk(string);

    string GbkToUtf8(string);

    wstring Utf8ToUnicode(string);

    wstring GbkToUnicode(string);

    string UnicodeToUtf8(wstring);

    string UnicodeToGbk(wstring);


    void Trim(string &str, char trim);

    bool startsWith(const std::string str, const std::string prefix);

    bool endsWith(const std::string str, const std::string suffix);

    void
    base64_encode(unsigned char *input, unsigned int input_length, unsigned char *output, unsigned int *output_length);

    void
    base64_decode(unsigned char *input, unsigned int input_length, unsigned char *output, unsigned int *output_length);

    string getIpStr(unsigned int ip);

    bool isIPv4(string IP);

    bool isIPv6(string IP);

    string validIPAddress(string IP);

    void GetDirFiles(string path, vector<string> &array);

    //创建路径文件夹
    void CreatePath(const std::string path);
};


#endif //_OS_H
