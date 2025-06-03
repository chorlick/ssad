#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#include "ssad.h"
#include "common.h"

void SSAD::port_listener(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        logger->write("ERROR", "Error opening socket on port %d", port);
        return;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        logger->write("ERROR", "Error binding on port %d", port);
        close(sockfd);
        return;
    }

    listen(sockfd, 5);

    while (running) {
        sockaddr_in cli_addr{};
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd >= 0) {
            string ip = inet_ntoa(cli_addr.sin_addr);
            tracker->record_knock(ip, port);
            close(newsockfd);
        }
    }

    close(sockfd);
}

int SSAD::run(const string& config_path) {
    Config config;
    if (!config.load(config_path)) {
        return 1;
    }

    logger = new Logger(config.log_file);
    Tracker tracker(config, *logger);
    vector<thread> listeners;

    for (int port : config.trigger_ports) {
        auto& log = logger;
        listeners.emplace_back([port, &tracker, &config, &log]() {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            bind(sock, (sockaddr*)&addr, sizeof(addr));
            listen(sock, 5);
            unordered_map<string, chrono::steady_clock::time_point> knock_map;

            while (running) {
                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(sock, &fds);
                
                timeval timeout {1, 0};

                if (!knock_map.empty()) {
                    auto now = chrono::steady_clock::now();
                    for (auto it = knock_map.begin(); it != knock_map.end(); ) {
                        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - it->second).count();
                        if (elapsed >= config.sequence_timeout_ms) {
                            it = knock_map.erase(it); // Expired
                        } else {
                            int remaining = config.sequence_timeout_ms - elapsed;
                            timeout.tv_sec = remaining / 1000;
                            timeout.tv_usec = (remaining % 1000) * 1000;
                            break; // not for multi hosts
                        }
                    }
                }                

                int sel = select(sock + 1, &fds, nullptr, nullptr, &timeout);
                if (sel > 0 && FD_ISSET(sock, &fds)) {
                    sockaddr_in client;
                    socklen_t len = sizeof(client);
                    int conn = accept(sock, (sockaddr*)&client, &len);
                    if (conn >= 0) {
                        string ip = inet_ntoa(client.sin_addr);
                        if (knock_map.find(ip) == knock_map.end()) {
                            knock_map[ip] = chrono::steady_clock::now();
                        }
                        tracker.record_knock(ip, port);
                        close(conn);
                    }
                }
            }
            close(sock);
        });
    }

    for (auto& t : listeners) {
        t.join();
    }

    return 0;
}


