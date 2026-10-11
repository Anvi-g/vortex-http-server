#include "Server.hpp"
#include <iostream>

int main() {
    try {
        Server server(8080);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "[Fatal Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
