#include "ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager() {}

ClientManager& ClientManager::getInstance() {
    static ClientManager instance;
    return instance;
}

void ClientManager::handleEvent(struct kevent& event) {
    Client *client = reinterpret_cast<Client*>(event.udata);
    if (event.filter == EVFILT_READ) {
        switch (client->status_) {
        case RECV_REQUEST: client->handleSocketReadEvent(); break;
        case READ_CGI: client->handleCgiReadEvent(event.ident); break;
        case READ_FILE: client->handleFileReadEvent(event.ident); break;
        }
    } else if (event.filter == EVFILT_WRITE) {
        switch (client->status_) {
        case SEND_RESPONSE: client->handleSocketWriteEvent(); break;
        case WRITE_CGI: client->handleCgiWriteEvent(event.ident); break;
        case WRITE_FILE: client->handleFileWriteEvent(event.ident); break;
        }
    }
    // if (client->status_ == DONE) // 해당 클라이언트 지우기
}
