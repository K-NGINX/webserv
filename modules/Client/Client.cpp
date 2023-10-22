#include "Client.hpp"

Client::Client(int client_socket) : socket_(client_socket), status_(INIT) {
    resource_fd_[0] = -1;
    resource_fd_[1] = -1;
}
