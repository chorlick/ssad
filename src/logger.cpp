#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

void Logger::write(const char* level, const char* format, ...) const{
    lock_guard<mutex> lock(log_mutex);
    ofstream log(path, ios::app);

    if (log) {
        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        log << "[" << put_time(localtime(&now), "%F %T") << "] " << level << ": ";

        char buffer[1024] = {0};
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        log << buffer << "\n";
    }
}

