#ifndef _SSAD_LOGGER_H_
#define _SSAD_LOGGER_H_

#include <string>
#include <mutex>
#include <ostream>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

enum class LOG_LEVEL {
    INFO,
    WARN, 
    ERROR, 
    DEBUG
};


/**
 * @class Logger
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

    const char* to_string(LOG_LEVEL level) const {
        switch (level) {
            case LOG_LEVEL::INFO:  return "INFO";
            case LOG_LEVEL::WARN:  return "WARN";
            case LOG_LEVEL::ERROR: return "ERROR";
            case LOG_LEVEL::DEBUG: return "DEBUG";
            default:              return "UNKNOWN";
        }
    }
    public:

    /**
    * @brief Constructs a Logger that writes to the specified log file.
    *
    * Initializes the logger with a path to the log file. All subsequent log messages
    * will be appended to this file.
    *
    * @param file_path The path to the file where log messages will be written.
    */
    Logger(const string& file_path) : path(file_path) {}
    

    /**
    * @brief Writes a formatted log message with a timestamp and severity level.
    *
    * This function appends a log entry to the configured log file. The message is timestamped
    * using the system clock and includes the provided log level (e.g., "INFO", "ERROR").
    * It uses `printf`-style formatting with a variable argument list.
    *
    * Thread-safe via a mutex to ensure serialized access to the log file.
    *
    * @param level The severity level of the message (e.g., "INFO", "ERROR").
    * @param format The `printf`-style format string.
    * @param ... Additional arguments corresponding to the format string.
    */
    void write(LOG_LEVEL level, const char* format, ...) const;
    
};

#endif