#include "ClientManager.hpp"

ClientManager& ClientManager::getInstance() {
    static ClientManager instance;
    return instance;
}

bool ClientManager::isClientSocket(int fd) {
    return m_client_.find(fd) != m_client_.end();
}
