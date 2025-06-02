#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <unordered_map>
#include <chrono>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <iostream>

#include "config.h"

using namespace std;

/**
 * @class KnockTracker
 * @brief Tracks per-IP port knock sequences and triggers activation logging.
 *
 * This class handles state management for IP addresses attempting to activate
 * the hidden service. It validates knock sequence order and timing constraints
 * as defined in the loaded configuration.
 */
class Tracker {
private:
    struct KnockState {
        size_t index = 0;
        chrono::steady_clock::time_point start_time;
        chrono::steady_clock::time_point last_knock;
    };

    const Config& config;
    unordered_map<string, KnockState> state_map;
    mutex state_mutex;

    void log_activation(const string& ip) const {
        ofstream log(config.log_file, ios::app);
        if (log) {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
            log << "[" << put_time(localtime(&now), "%F %T") << "] Service activated for IP: " << ip << "\n";
        }
    }

public:
    Tracker(const Config& cfg) : config(cfg) {}

    void record_knock(const string& ip, int port) {
        lock_guard<mutex> lock(state_mutex);
        auto now = chrono::steady_clock::now();

        auto& state = state_map[ip];

        if (state.index == 0 || chrono::duration_cast<chrono::milliseconds>(now - state.start_time).count() > config.sequence_timeout_ms) {
            state.index = 0;
            state.start_time = now;
        }

        if (state.index > 0 && chrono::duration_cast<chrono::milliseconds>(now - state.last_knock).count() > config.inter_knock_timeout_ms) {
            state.index = 0;
        }

        if (port == config.trigger_ports[state.index]) {
            state.last_knock = now;
            state.index++;

            if (state.index == config.trigger_ports.size()) {
                log_activation(ip);
                state_map.erase(ip);
            }
        } else {
            state.index = 0;
        }
    }
};

#endif