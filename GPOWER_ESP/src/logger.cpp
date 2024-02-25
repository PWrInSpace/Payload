#include "logger.h"

void Logger::setLogLevel(LogLevel level){
    Serial.begin(115200);
    this->level = level;
}

void Logger::info(String message, String module = "") {
    if(this->level >= INFO){
        String output = "[INFO " + module + "] " + message;
        Serial.println(output);
    }
}

void Logger::warn(String message, String module = "") {
    if(this->level >= WARN){
        String output = "[WARNING " + module + "] " + message;
        Serial.println(output);
    }
}

void Logger::error(String message, String module = "") {
    if(this->level >= WARN){
        String output = "[ERROR " + module + "] " + message;
        Serial.println(output);
    }
}