

#include "tracker.h"

void Tracker::record_knock(const string& ip, int port) {
    std::lock_guard<std::mutex> lock(state_mutex);  
    auto now = chrono::steady_clock::now();

    auto& state = state_map[ip];

    //reset timer and sequence if the timeout has passed
    if (state.index == 0 || chrono::duration_cast<chrono::milliseconds>(now - state.start_time).count() > config.sequence_timeout_ms) {
        state.index = 0;
        state.start_time = now;
    }

    //reset sequence if the knock timer has passed
    if (state.index > 0 && chrono::duration_cast<chrono::milliseconds>(now - state.last_knock).count() > config.inter_knock_timeout_ms) {
        state.index = 0;
    }

    //check to see if its the correct port & update the state
    if (port == config.trigger_ports[state.index]) {
        state.last_knock = now;
        state.index++;
        
        //check if the sequence is good. log. reset
        if (state.index == config.trigger_ports.size()) {
            log_activation(ip);
            state_map.erase(ip);
        }

    } else {
        //reset the whole sequence
        state.index = 0;
    }
}

void Tracker::log_activation(const string& ip) const {
    logger.write(LOG_LEVEL::INFO, "service activation <%s>", ip.c_str());
}