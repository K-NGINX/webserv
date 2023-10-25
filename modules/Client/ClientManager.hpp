#pragma once

#include <sys/event.h>

#include "Client.hpp"

class ClientManager {
public:
	static ClientManager& getInstance();

	bool isClientSocket(int fd);
	void addClient(int fd);
	void handleReadEvent(struct kevent event);

private:
	ClientManager();
	~ClientManager();

	std::map<int, Client> m_client_;
};
