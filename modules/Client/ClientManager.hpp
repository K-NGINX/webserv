#pragma once

#include <sys/event.h>
#include <vector>

#include "Client.hpp"

class ClientManager {
public:
    static ClientManager& getInstance();
    void handleClientSocketEvent(struct kevent& event);
    void handleCgiEvent(struct kevent& event);
    void handleFileEvent(struct kevent& event);

    std::vector<Client> v_client_;

private:
    ClientManager();
    ~ClientManager();

    Client& getClientBySocket(int socket);
    Client& getClientByResourceFd(int fd);
};