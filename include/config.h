#ifndef _SSAD_CONFIG_H_
#define _SSAD_CONFIG_H_

#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "logger.h"

using namespace std;

/**
 * @class Config
 * @brief Loads and validates daemon configuration from a JSON file.
 *
 * Responsible for parsing JSON configuration and validating that required
 * fields are present and correctly formatted. Also owns the error logger instance.
 */
class Config {

public:
    //these ar
    vector<int> trigger_ports;
    int sequence_timeout_ms;
    int inter_knock_timeout_ms;
    string log_file;

    bool load(const string& path);
};

#endif