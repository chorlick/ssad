#ifndef _SSAD_SSAD_H_
#define _SSAD_SSAD_H_

#include <thread>
#include <atomic>
#include <iostream>
#include <memory>

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
 *
 * Functions as a singleton
 */
class SSAD {
private:
    Config config;
    unique_ptr<Logger> logger;
    unique_ptr<Tracker> tracker;
    vector<thread> threads;

    static SSAD* instance;

    /**
    * @brief Starts a blocking TCP listener on a specific port.
    *
    * This function opens a socket on the specified port, listens for incoming
    * TCP connections, and forwards each connection's source IP to the `Tracker`
    * instance to record the knock attempt. It runs in a blocking loop and handles
    * one port per thread.
    *
    * @param port The TCP port number to listen on.
    */    
    void port_listener(int port);

public:
    SSAD() { instance = this; }
    ~SSAD() { }

    /**
    * @brief Main entry point for the Stealth Service Activation Daemon.
    *
    * Loads the configuration file, initializes the logger and port knock tracker,
    * and launches multithreaded listeners for each port specified in the config.
    * Each thread monitors a TCP port, accepts client connections, and passes IPs
    * to the `Tracker` for sequence validation.
    *
    * Uses `select()` with a dynamically adjusted timeout to periodically check and
    * expire knock sequences per IP if the timeout is exceeded.
    *
    * @param config_path Path to the JSON configuration file.
    * @return int Returns 0 on successful startup and shutdown; returns 1 if configuration fails to load.
    */    
    int run(const string& config_path);
};

#endif