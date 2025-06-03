


#include "config.h"

#ifdef __linux__
#define DEFAULT_PATH ("/var/log/ssad_activations.log")
#endif

bool Config::load(const string& path) {
    
    if(!std::filesystem::exists(path)) {
        cerr << "no such file for config_path " << path << endl;
        return false;
    }

    ifstream f(path);
    if (!f.is_open()) {
        cerr << "unable to load configuration file for " << path << endl;
        return false;
    } 

    nlohmann::json j;
    try {
        f >> j;

        if (!j.contains("trigger_ports") || !j["trigger_ports"].is_array()) {
            throw runtime_error("configuration file must include trigger_ports. check the schema file");
        }

        if (!j.contains("sequence_timeout_ms") || !j["sequence_timeout_ms"].is_number_integer()) {
            throw runtime_error("configuration file must include sequence_timeout_ms and should be ints (0 - 65k). check the shcema file");
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
        //we had to do something funny here...if it wasnt able to load the config
        //file we dont really know where to log it. lets do a hail mary and throw
        //it at the default location
        Logger logger(DEFAULT_PATH);  
        logger.write(LOG_LEVEL::ERROR, "%s", e.what());   
        return false;
    }

    return true;
}