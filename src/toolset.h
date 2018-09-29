#ifndef __VPNTOOL_H__
#define __VPNTOOL_H__

#include "lib/command_factory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <map>

enum {
    Debug = 0,
    Info,
    Warning,
    Error,
};

#define CONSOLE_Debug(fmt, ...)                                     \
    do {                                                            \
        consoleLog(Debug, "[%d][%s:%s:%d] " fmt,                    \
                   (int)getpid(), __FILE__, __FUNCTION__, __LINE__, \
                   ##__VA_ARGS__);                                  \
    } while(0)

#define CONSOLE_Info(fmt, ...)                          \
        do {                                            \
            consoleLog(Info, "[%d] " fmt,               \
                       (int)getpid(), ##__VA_ARGS__);   \
        } while(0)

#define CONSOLE_Warning(fmt, ...)                   \
    do {                                            \
        consoleLog(Warning, "[%d] " fmt,            \
                   (int)getpid(), ##__VA_ARGS__);   \
    } while(0)

#define CONSOLE_Error(fmt, ...)                     \
    do {                                            \
        consoleLog(Error, "[%d] " fmt,              \
                   (int)getpid(), ##__VA_ARGS__);   \
    } while(0)

#define CONSOLE(level, fmt, ...) CONSOLE_##level(fmt, ##__VA_ARGS__)

// 获取程序名
const char *programName();

static inline void consoleLog(size_t level, const char *fmt, ...)
{
    char buf[256] = {0};
    va_list ap;

    static const char *LEVEL_STR[] = {
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
    };

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    struct tm ltm;
    time_t logtime;

    logtime = time(NULL);
    localtime_r(&logtime, &ltm);

    fprintf(level >= Warning ? stderr : stdout,
            "[%04d-%02d-%02d %02d:%02d:%02d][%s]%s\n",
            ltm.tm_year + 1990, ltm.tm_mon + 1, ltm.tm_mday,
            ltm.tm_hour, ltm.tm_min, ltm.tm_sec,
            level < sizeof(LEVEL_STR)/sizeof(LEVEL_STR[0]) ? LEVEL_STR[level] : "Unknown",
            buf);
}

template<typename T>
inline void split(const std::basic_string< T >& s, T c, std::vector< std::basic_string< T > > &v)
{
    if(s.size() == 0)
        return;

    typename std::basic_string< T >::size_type i = 0;
    typename std::basic_string< T >::size_type j = s.find(c);

    while(j != std::basic_string<T>::npos) {
        std::basic_string<T> buf = s.substr(i, j - i);

        if(buf.size() > 0)
            v.push_back(buf);

        i = ++j; j = s.find(c, j);
    }

    if(j == std::basic_string<T>::npos) {
        std::basic_string<T> buf = s.substr(i, s.length() - i);
        if(buf.size() > 0)
            v.push_back(buf);
    }
}

#endif // __VPNTOOL_H__
