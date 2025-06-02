#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ssad.h"


void SSAD::signal_handler(int) {
        if (instance) {
            instance->running = false;
    }
}

void SSAD::port_listener(int port) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            cerr << "Error opening socket on port " << port << endl;
            return;
        }

        sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            cerr << "Error binding on port " << port << endl;
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

public:
    SSADaemon() {
        instance = this;
    }

    ~SSADaemon() {
        delete tracker;
    }

    int run(const string& config_path) {
        if (!config.load(config_path)) {
            cerr << "Failed to load config file\n";
            return 1;
        }

        tracker = new Tracker(config);

        signal(SIGINT, signal_handler);

        for (int port : config.trigger_ports) {
            threads.emplace_back(&SSADaemon::port_listener, this, port);
        }

        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }

        return 0;
    }
};

