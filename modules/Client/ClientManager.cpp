#include "ClientManager.hpp"

ClientManager& ClientManager::getInstance() {
	static ClientManager instance;
	return instance;
}

bool ClientManager::isClientSocket(int fd) {
	return m_client_.find(fd) != m_client_.end();
}

void ClientManager::addClient(int fd) {
	Client client(fd);
	// if (m_client_.find(fd) != m_client_.end())
	m_client_[fd] = client;
}

/**
 * @brief 클라이언트 소켓에 발생한 read 이벤트를 처리하는 함수
 * 
 * @param event read 이벤트가 발생한 이벤트 구조체
 */
void ClientManager::handleReadEvent(struct kevent event) {
	Client& client = m_client_[event.ident];

	switch (client.getStatus()) {
	case INIT:
		client.setStatus(PARSE_REQUEST);
	case PARSE_REQUEST:
	case READ_RESOURCE:
	case MAKE_RESPONSE:
	case WRITE_RESOURCE:
	case SEND_RESPONSE:
	case DONE:
	}
}