#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "MimeTypes.hpp"    

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>


using namespace std;
namespace fs = std::filesystem;

Server::Server(int port, const string& public_dir) 
    : port_(port), server_fd_(-1), is_running_(false), public_dir_(public_dir) {
    if (!fs::exists(public_dir_) && fs::exists("../" + public_dir_)) {
        public_dir_ = "../" + public_dir_;
    }
}


Server::~Server(){ stop(); }

void Server::init_socket(){
    // IPv4 TCP socket
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd_ < 0){
        throw runtime_error("Failed to create socket: " + string(strerror(errno)));
    }

    // enable SO_REUSEADDR -> port freed immidiately on start
    int opt = 1;
    if(setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        close(server_fd_);
        server_fd_ = -1;
        throw runtime_error("setsockopt failed: " + string(strerror(errno)));
    }

    // bind address and port
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //0.0.0.0
    server_addr.sin_port = htons(port_); //to network byte order

    if(bind(server_fd_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0){
        close(server_fd_);
        throw runtime_error("Bind failed on port: " + to_string(port_) + ": " + string(strerror(errno)));
    }

    // listen
    const int BACKLOG = 128;
    if(listen(server_fd_, BACKLOG) < 0){
        close(server_fd_);
        server_fd_ = -1;
        throw runtime_error("Listen failed: " + string(strerror(errno)));
    }
}

void Server::start(){
    init_socket();
    is_running_ = true;
    cout << "[Vortex Server] Started on port: " << port_ << "...\n";

    // infinite loop accepting connections
    while(is_running_){
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd_, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if(client_fd < 0){
            if(is_running_){
                cerr << "[Vortex] Accept failed: " << strerror(errno) << "\n";
            }
            continue;
        }

        // print client info
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        cout << "[Vortex] Connection accepted from : " << client_ip << ":" << ntohs(client_addr.sin_port) << "\n";

        // handle client request (asynchronous)
        std::thread(&Server::handle_client, this, client_fd).detach();
    }
}

void Server::handle_client(int client_fd){
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    // read request
    ssize_t bytes_recieved = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if(bytes_recieved > 0){
        buffer[bytes_recieved] = '\0';

        // parse request
        HttpRequest request = HttpRequest::parse(buffer);
        cout << "[Vortex] " << request.method << " " << request.path << "\n";

        HttpResponse response;

        // reject unsupported methods
        if(request.method != "GET"){
            response = HttpResponse(405, "Method Not Allowed");
            response.set_body("405 Method Not Allowed\n", "text/plain");
        }
        else if(request.path == "/health"){
            response = HttpResponse(200, "OK");
            response.set_body("{\"status\":\"healthy\",\"uptime\":\"ok\"}\n", "application/json");
        }
        // simulated heavy / slow operation
        else if (request.path == "/slow") {
            cout << "[Vortex] Simulating 5-second long-running request on Thread " 
              << this_thread::get_id() << "...\n";
            this_thread::sleep_for(chrono::seconds(5));
            response = HttpResponse(200, "OK");
            response.set_body("<h1>Finished slow operation!</h1>\n", "text/html");
        }
        else {
              // resolve requested path (default '/' to '/index.html')
            string req_path = request.path;
            if (req_path == "/") {
                req_path = "/index.html";
            }
            // construct filesystem path
            fs::path base_path = fs::absolute(public_dir_);
            fs::path full_path = fs::absolute(base_path.string() + req_path);

            // path traversal prevention
            // ensure full_path starts with base_path
            string canonical_base = base_path.lexically_normal().string();
            string canonical_full = full_path.lexically_normal().string();
            if (canonical_full.rfind(canonical_base, 0) != 0) {
                // path traversal detected!
                response = HttpResponse(403, "Forbidden");
                response.set_body("<h1>403 Forbidden: Path Traversal Detected</h1>\n", "text/html");
            }
            // check if file exists and is a regular file
            else if (!fs::exists(full_path) || !fs::is_regular_file(full_path)) {
                response = HttpResponse(404, "Not Found");
                response.set_body("<h1>404 Not Found</h1>\n", "text/html");
            }
            // read binary file from disk
            else {
                ifstream file(full_path, ios::binary);
                if (file) {
                    string file_content(
                        (istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>()
                    );
                    std::string mime_type = MimeTypes::get_mime_type(full_path.string());
                    response = HttpResponse(200, "OK");
                    response.set_body(file_content, mime_type);
                } else {
                    response = HttpResponse(500, "Internal Server Error");
                    response.set_body("500 Internal Server Error\n", "text/plain");
                }
            }
        }


        // send serialise response
        string response_str = response.to_string();
        send(client_fd, response_str.c_str(), response_str.size(), 0);
    }
    close(client_fd);    
}

void Server::stop() {
    if (is_running_) {
        is_running_ = false;
    }
    if (server_fd_ != -1) {
        close(server_fd_);
        server_fd_ = -1;
        std::cout << "[Vortex] Server stopped and socket closed.\n";
    }
}