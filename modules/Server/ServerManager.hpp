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
#include "Kqueue/Kqueue.hpp"

class ServerManager {
public:
	static ServerManager& getInstance();

	void init();
	void start();

private:
	ServerManager();
	~ServerManager();

	void closeAllServerFd();

	std::vector<int> v_server_fd_;
	Kqueue kqueue_;
};
