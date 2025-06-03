#ifndef _SSAD_SSAD_H_
#define _SSAD_SSAD_H_

#include <thread>
#include <atomic>

#include "config.h"
#include "tracker.h"

/**
 * @class SSAD
 * @brief Main orchestrator for the Stealth Service Activation Daemon (SSAD).
 *
 * Responsible for initializing configuration, setting up signal handling,
 * spawning listener threads on each trigger port, and coordinating shutdown.
 * Each port is monitored in a separate thread. When SIGINT is received, the
 * daemon stops all threads and exits cleanly.
 */
class SSAD {
private:
    Config config;
    Logger * logger;
    Tracker * tracker = nullptr;
    vector<thread> threads;

    static SSAD* instance;
    void port_listener(int port);

public:
    SSAD() { instance = this; }
    ~SSAD() { 
        if(tracker) {
            delete tracker; 
        }

        if(logger) {
            delete logger;
        }
    }
    int run(const string& config_path);
};

#endif