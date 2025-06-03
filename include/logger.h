#ifndef _SSAD_LOGGER_H_
#define _SSAD_LOGGER_H_

#include <string>
#include <mutex>
#include <ostream>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

/**
 * @class LogFile
 * @brief Thread-safe logger for writing timestamped log messages to a file.
 *
 * This class encapsulates all file I/O for logging messages to disk.
 * It uses a mutex to ensure that concurrent writes from multiple threads
 * are safely serialized.
 */
class Logger {
private:
    string path;
    mutable mutex log_mutex;

public:
    Logger(const string& file_path) : path(file_path) {}
    void write(const char* level, const char* format, ...) const;
};

#endif