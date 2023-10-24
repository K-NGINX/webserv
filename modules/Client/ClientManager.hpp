#pragma once

#include "Client.hpp"

class ClientManager {
public:
	static ClientManager& getInstance();

	bool isClientSocket(int fd);
	void addClient(int fd);
private:
	ClientManager();
	~ClientManager();

	std::map<int, Client> m_client_;
};
