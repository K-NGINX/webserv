#pragma once

#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <sys/event.h>
#include <sys/socket.h> // server.cpp
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>


#include "../Config/ConfigManager.hpp"

class ServerManager {
public:
	static ServerManager& getInstance();
	void initServer();
	void initKqueue();
	void closeAllServerFd();
private:
	ServerManager();
	~ServerManager();
	std::vector<int> v_server_fd_;
	int kqueue_fd_;
	// std::vector<int> v_new_socket_fd_; // accept해서 내놓을 소켓
};
