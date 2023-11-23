#pragma once

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include "../Client/Client.hpp"
#include "../Config/Block/ServerBlock.hpp"
#include "../Config/ConfigManager.hpp"
#include "Kqueue/Kqueue.hpp"

class ClientManager;

class ServerManager {
   public:
	static ServerManager &getInstance();

	void init();
	void start();

	Kqueue kqueue_;

   private:
	ServerManager();
	~ServerManager();

	void closeAllServerSocket();
	void initServerSocket(const ServerBlock &server_block);
	void handleEvent(struct kevent &event);
	bool isServerSocket(int fd);
	void connectNewClient(int server_fd);

	std::vector<int> v_server_socket_;
};
