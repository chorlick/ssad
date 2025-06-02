#include "logger.h"

void Logger::write(const string& level, const string& message) const{
    lock_guard<mutex> lock(log_mutex);
    ofstream log(path, ios::app);
    if (log) {
        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        log << "[" << put_time(localtime(&now), "%F %T") << "] " << level << ": " << message << "\n";
    }
}