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

    bool load(const string& path) {
        ifstream f(path);
        if (!f.is_open()) return false;

        nlohmann::json j;
        try {
            f >> j;

            if (!j.contains("trigger_ports") || !j["trigger_ports"].is_array()) {
                throw runtime_error("configuration file must include trigger_ports. check the schema file");
            }

            if (!j.contains("sequence_timeout_ms") || !j["sequence_timeout_ms"].is_number_integer()) {
                throw runtime_error("configuration file must include sequence_timeout_m and should be ints (0 - 65k). check the shcema file");
            }

            if (!j.contains("inter_knock_timeout_ms") || !j["inter_knock_timeout_ms"].is_number_integer()) {
                throw runtime_error("configuration file must include inter_knock_timeout_ms. check schema file");
            }

            if (!j.contains("activation_log_file") || !j["activation_log_file"].is_string()){
                throw runtime_error("configuration file must include activation_log_file. check schema file");
            }

            auto ports = j["trigger_ports"].get<vector<int>>();
            if (ports.size() < 3 || ports.size() > 5) throw runtime_error("trigger_ports must contain between 3 and 5 entries");
            for (int p : ports) {
                if (p < 1 || p > 65535) throw runtime_error("trigger_ports contains invalid port numbers");
            }

            trigger_ports = ports;
            sequence_timeout_ms = j["sequence_timeout_ms"].get<int>();
            inter_knock_timeout_ms = j["inter_knock_timeout_ms"].get<int>();
            log_file = j["activation_log_file"].get<string>();

        } catch (const exception& e) {
            Logger logger("/var/log/ssad_activations.log");  
            logger.write("ERROR", "%s", e.what());   
            return false;
        }

        return true;
    }
};

#endif