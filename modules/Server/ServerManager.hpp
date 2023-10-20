#pragma once

#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h> // server.cpp
#include <unistd.h>
#include <vector>

#include "Config/ConfigManager.hpp"

class ServerManager {
public:
	static ServerManager& getInstance();
	void initServer();

private:
	ServerManager();
	~ServerManager();
	std::vector<int> v_server_socket_fd_;
	// std::vector<int> v_new_socket_fd_; // accept해서 내놓을 소켓
};
