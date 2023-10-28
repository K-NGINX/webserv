#pragma once

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>

#include <algorithm>
#include <cstdlib>
// #include <string>
#include <sys/event.h>
#include <sys/socket.h>	   // server.cpp
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
// #include <vector>

#include "../Client/ClientManager.hpp"
#include "../Utils/Utils.hpp"
#include "Kqueue/Kqueue.hpp"

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
	void handleEvent(struct kevent &event);
	bool isServerSocket(int fd);
	void connectNewClient(int server_fd);

	std::vector<int> v_server_socket_;
};
