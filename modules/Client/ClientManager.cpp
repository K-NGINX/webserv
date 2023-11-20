#include "ClientManager.hpp"

ClientManager::ClientManager() {}
ClientManager::~ClientManager() {}

ClientManager &ClientManager::getInstance() {
	static ClientManager instance;
	return instance;
}

void ClientManager::disconnectClient(Client *client) {
	ServerManager::getInstance().kqueue_.stopMonitoringWriteEvent(client->socket_);
	std::cout << RED << "\n❗️ CLIENT(" << client->socket_ << ") DISCONNECTED" << RESET << std::endl;
	std::vector<Client *>::iterator client_it = v_client_.begin();
	while (client_it != v_client_.end()) {
		if ((*client_it)->socket_ == client->socket_) {
			v_client_.erase(client_it);
			break;
		}
		client_it++;
	}
	delete client;
}

/**
 * @brief 클라이언트 소켓, CGI fd, 파일 fd에서 발생한 이벤트를 처리 하는 함수
 *
 * @param event 해당 이벤트
 */
void ClientManager::handleEvent(struct kevent &event) {
	Client *client = reinterpret_cast<Client *>(event.udata);
	if (client == NULL) // 이벤트 감지가 중단된 fd일 때
		return;

	if (event.filter == EVFILT_READ) {
		switch (client->status_) {
			case RECV_REQUEST:
				client->handleSocketReadEvent();
				break;
			case READ_CGI:
				client->handleCgiReadEvent();
				break;
			case READ_FILE:
				client->handleFileReadEvent();
				break;
			default:
				break;
		}
	} else if (event.filter == EVFILT_WRITE) {
		switch (client->status_) {
			case SEND_RESPONSE:
				client->handleSocketWriteEvent();
				break;
			case WRITE_CGI:
				client->handleCgiWriteEvent();
				break;
			default:
				break;
		}
	}

	// 요청을 받고 응답을 보내는 일련의 과정을 마쳤다면, 클라이언트와 연결 끊어주기
	if (client->status_ == WILL_DISCONNECT) disconnectClient(client);
}
