// ssad_daemon.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>

#include <atomic>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

#include "config.h"
#include "tracker.h"
#include "ssad.h"

using json = nlohmann::json;
using namespace std;


SSAD* SSAD::instance = nullptr;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <config_file>\n";
        return 1;
    }
    SSAD daemon;
    return daemon.run(argv[1]);
}
