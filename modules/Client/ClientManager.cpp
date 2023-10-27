#include "ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager() {}

ClientManager& ClientManager::getInstance() {
    static ClientManager instance;
    return instance;
}

void ClientManager::disconnectClient(Client* client) {
	std::cout << MAGENTA << "\nCLIENT(" << client->socket_ << ") DISCONNECTED" << RESET << std::endl;
    std::vector<Client>::iterator client_it = v_client_.begin();
    while (client_it != v_client_.end()) {
        if (client_it->socket_ == client->socket_) {
            v_client_.erase(client_it);
            break;
        }
        client_it++;
    }
}

void ClientManager::handleEvent(struct kevent& event) {
    Client *client = reinterpret_cast<Client*>(event.udata);
    if (event.filter == EVFILT_READ) {
        switch (client->status_) {
        case RECV_REQUEST: client->handleSocketReadEvent(); break;
        case READ_CGI: client->handleCgiReadEvent(event.ident); break;
        case READ_FILE: client->handleFileReadEvent(event.ident); break;
        default: break;
        }
    } else if (event.filter == EVFILT_WRITE) {
        switch (client->status_) {
        case SEND_RESPONSE: client->handleSocketWriteEvent(); break;
        case WRITE_CGI: client->handleCgiWriteEvent(event.ident); break;
        case WRITE_FILE: client->handleFileWriteEvent(event.ident); break;
        default: break;
        }
    }
    // 클라이언트가 요청을 받고 응답을 보내는 일련의 과정을 마쳤다면, 연결 끊어주기
    if (client->status_ == WILL_DISCONNECT)
        disconnectClient(client);
}
