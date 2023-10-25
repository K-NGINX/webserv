#include "ClientManager.hpp"

ClientManager& ClientManager::getInstance() {
	static ClientManager instance;
	return instance;
}

void ClientManager::addClient(int fd) {
	m_client_[fd] = Client(fd);
}

Client& ClientManager::getClient(struct kevent event) {
	std::map<int, Client>::iterator client_it = m_client_.begin();
	while (client_it != m_client_.end()) {
		if (client_it->first == event.ident // 클라이언트 소켓
		|| client_it->second.getReadResourceFd() == event.ident) // 리소스 읽기 fd
			return client_it->second;
		client_it++;
	}
	throw std::runtime_error("client 없음"); // 불릴 일 없음
}

/**
 * @brief 클라이언트 소켓에 발생한 read 이벤트를 처리하는 함수
 * 
 * @param event read 이벤트가 발생한 이벤트 구조체
 */
void ClientManager::handleReadEvent(struct kevent event) {
	try {
		Client& client = getClient(event);

		switch (client.getStatus()) {
		case PARSE_REQUEST:
			client.parseRequest();
			break;
		case READ_RESOURCE:
			client.readResponse();
			break;
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}