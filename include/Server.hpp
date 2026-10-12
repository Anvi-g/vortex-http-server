#pragma once
#include <string> 
#include <cstdint>
#include <thread>


class Server {
private:
    int port_;
    int server_fd_;
    bool is_running_;
    std::string public_dir_;

    void init_socket();
    void handle_client(int client_fd);

public:
    explicit Server(int port, const std::string& public_dir = "./public");

    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void start();
    void stop();
};
