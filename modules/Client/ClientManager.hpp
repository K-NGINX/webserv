#pragma once

#include <sys/event.h>

#include "Client.hpp"
class Client;

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
