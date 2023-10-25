#include "Client.hpp"

Client::Client(int client_socket) : socket_(client_socket), status_(PARSE_REQUEST) {
    resource_fd_[0] = -1;
    resource_fd_[1] = -1;
}

const ClientStatus& Client::getStatus() const { return status_; }

const int& Client::getReadResourceFd() const { return resource_fd_[0]; }

void Client::setStatus(const ClientStatus& status) { status_ = status; }

void Client::parseRequest() {
    // 파싱
    // if (request_.getStatus() == DONE || request_.getStatus() == ERROR)
        makeResponse();
}

void Client::makeResponse() {

}

void Client::readResponse() {

}
