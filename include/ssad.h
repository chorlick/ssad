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
    Tracker * tracker = nullptr;
    Logger * logger = nullptr;
    vector<thread> threads;
    atomic<bool> running{true};

    static SSAD* instance;
    static void signal_handler(int);
    void port_listener(int port);

public:
    SSAD() { instance = this; }
    ~SSAD() { delete tracker; }
    int run(const string& config_path);
};

#endif