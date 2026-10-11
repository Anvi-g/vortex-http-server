#pragma once //prevents multiple file compilations
#include <string> 
#include <cstdint>

class Server{
    private:
    int port_;
    int server_fd_;
    bool is_running_;

    void init_socket();
    void handle_client(int client_fd);

    public:
    explicit Server(int port);

    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void start();
    void stop();
};