#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <unordered_map>
#include <chrono>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <iostream>

#include "config.h"
#include "knockstate.h"
#include "logger.h"

using namespace std;

/**
 * @class Tracker
 * @brief Tracks per-IP port knock sequences and triggers activation logging.
 *
 * This class handles state management for IP addresses attempting to activate
 * the hidden service. It validates knock sequence order and timing constraints
 * as defined in the loaded configuration.
 */
class Tracker {
private:

    const Config& config;
    Logger& logger;
    unordered_map<string, KnockState> state_map;
    mutex state_mutex;

    /**
    * @brief Logs a successful service activation for a given IP address.
    *
    * This function is called when a valid port knock sequence has been completed.
    * It logs the activation event using the logger instance.
    *
    * @param ip The IP address that completed the valid knock sequence and triggered activation.
    */
    void log_activation(const string& ip) const;

public:

    /**
    * @brief Constructs a Tracker object with the given configuration and logger.
    *
    * Initializes the port knock tracking system using the specified configuration parameters
    * and logger instance for recording events.
    *
    * @param cfg A reference to the configuration object containing knock sequence settings.
    * @param log A reference to the logger used for logging activation and debug messages.
    */
    Tracker(const Config& cfg, Logger& log) : config(cfg), logger(log)  {}

    /**
    * @brief Destroys the Tracker object.
    *
    * Cleans up any resources held by the Tracker. This destructor is trivial as no
    * dynamic memory management is required.
    */
    ~Tracker() {}

    /**
    * @brief Processes a port knock from a specific IP address and updates the knock sequence state.
    *
    * This function handles incoming port knocks and determines whether the configured port knock 
    * sequence has been completed in time and in the correct order. If a valid sequence is detected, 
    * it triggers the activation process for the IP address and removes the tracking state.
    *
    * @param ip The IP address from which the port knock is received.
    * @param port The port that was knocked.
    * @return std::lock_guard<std::mutex> A lock guard that holds the state mutex for thread-safe access.
    *
    */
    void record_knock(const string& ip, int port);

};

#endif