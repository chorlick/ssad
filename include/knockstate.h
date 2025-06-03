#ifndef _SSAD_KNOCK_STATE_H_
#define _SSAD_KNOCK_STATE_H_

#include  <chrono>

struct KnockState {
    size_t index = 0;
    chrono::steady_clock::time_point start_time;
    chrono::steady_clock::time_point last_knock;
};

#endif