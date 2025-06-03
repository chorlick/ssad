#ifndef _SSAD_KNOCK_STATE_H_
#define _SSAD_KNOCK_STATE_H_

#include  <chrono>

/**
 * @struct KnockState
 * @brief Represents the state of a port knock sequence for a single IP address.
 *
 * This structure tracks the progress of a port knocking sequence, including the current
 * index in the sequence, the time the sequence started, and the time of the most recent knock.
 */
struct KnockState {
    size_t index = 0;
    chrono::steady_clock::time_point start_time;
    chrono::steady_clock::time_point last_knock;
};

#endif