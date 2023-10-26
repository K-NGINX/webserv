#include "ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager() {}

ClientManager& ClientManager::getInstance() {
    static ClientManager instance;
    return instance;
}

Client& ClientManager::getClientBySocket(int socket) {
    std::vector<Client>::iterator client_it = v_client_.begin();
    while (client_it != v_client_.end()) {
        if (client_it->socket_ == socket)
            return *client_it;
        client_it++;
    }
}

Client& ClientManager::getClientByResourceFd(int fd) {
    std::vector<Client>::iterator client_it = v_client_.begin();
    while (client_it != v_client_.end()) {
        if (client_it->resource_fd_[0] == fd || client_it->resource_fd_[1] == fd)
            return *client_it;
        client_it++;
    }
}

void ClientManager::handleClientSocketEvent(struct kevent& event) {
    Client& client = getClientBySocket(event.ident);
    if (event.filter == EVFILT_READ)
        client.handleSocketReadEvent();
    else if (event.filter == EVFILT_WRITE)
        client.handleSocketWriteEvent();
}

void ClientManager::handleCgiEvent(struct kevent& event) {
    Client& client = getClientByResourceFd(event.ident);
    if (event.filter == EVFILT_READ)
        client.handleCgiReadEvent();
    else if (event.filter == EVFILT_WRITE)
        client.handleCgiWriteEvent();
    else if (event.filter == EVFILT_TIMER)
        client.handleCgiTimeoutEvent();
}

void ClientManager::handleFileEvent(struct kevent& event) {
    Client& client = getClientByResourceFd(event.ident);
    if (event.filter == EVFILT_READ)
        client.handleFileReadEvent();
    else if (event.filter == EVFILT_WRITE)
        client.handleFileWriteEvent();
}
