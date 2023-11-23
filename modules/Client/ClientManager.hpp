#pragma once

#include <sys/event.h>

#include <iostream>
#include <vector>

#include "../Server/ServerManager.hpp"
#include "Client.hpp"

class ClientManager {
   public:
	static ClientManager& getInstance();

	void handleEvent(struct kevent& event);

	std::vector<Client*> v_client_;

   private:
	ClientManager();
	~ClientManager();

	void disconnectClient(Client* client);
};
