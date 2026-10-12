#pragma once
#include <string> 
#include <cstdint>

class Server {
private:
    int port_;
    int server_fd_;
    bool is_running_;
    std::string public_dir_; // <-- use std::string

    void init_socket();
    void handle_client(int client_fd);

public:
    // <-- use std::string here as well
    explicit Server(int port, const std::string& public_dir = "./public");

    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void start();
    void stop();
};
