#ifndef _SSAD_CONFIG_H_
#define _SSAD_CONFIG_H_

#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>

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

    /**
    * @brief Loads the SSAD configuration from a JSON file.
    *
    * This function reads a configuration file from the specified path and initializes
    * the configuration parameters used by the port-knocking system.
    *
    * Expected fields in the JSON:
    * - `"trigger_ports"`: An array of 3 to 5 valid TCP port numbers (1–65535).
    * - `"sequence_timeout_ms"`: An integer timeout (in milliseconds) for the full sequence.
    * - `"inter_knock_timeout_ms"`: An integer timeout (in milliseconds) between individual knocks.
    * - `"activation_log_file"`: A string specifying the file path to the activation log.
    *
    * If the file is missing required fields or contains invalid values, an error is logged
    * and the function returns false.
    *
    * @param path The file path to the JSON configuration file.
    * @return true if configuration was successfully loaded and validated; false otherwise.
    */
    bool load(const string& path);
};

#endif