#ifndef LOGGER_H
#define LOGGER_H

#include "Arduino.h"

enum LogLevel {
    NONE = 0,
    ERROR,
    WARN,
    INFO
};

class Logger{

    LogLevel level = NONE;

    public:

        void setLogLevel(LogLevel);

        void info(String message, String module);
        void warn(String message, String module);
        void error(String message, String module);

};

extern Logger lg;

#endif