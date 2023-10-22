#pragma once

#include <iostream>
#include <map>

#include "Request/Request.hpp"
// #include "Response/Response.hpp"

enum ClientStatus {
	INIT,
	PARSE_REQUEST,
	READ_RESOURCE,
	MAKE_RESPONSE,
	SEND_RESPONSE,
	DONE
};

class Client {
public:
	Client(int client_socket);

private:
	int socket_; // 클라이언트 소켓
	int resource_fd_[2]; // 0: read, 1: write
	ClientStatus status_;
	Request request_;
	// Response response_;
};
