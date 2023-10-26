#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>

#include "Request/Request.hpp"
#include "Response/Response.hpp"

enum ClientStatus {
	PARSE_REQUEST,
	READ_RESOURCE,
	MAKE_RESPONSE, ////
	WRITE_RESOURCE,
	SEND_RESPONSE,
	DONE
};

class Client {
public:
	Client(int client_socket);

	/* getter */
	const ClientStatus& getStatus() const;
	const int& getReadResourceFd() const;

	/* setter */
	void setStatus(const ClientStatus& status);

	void parseRequest();
	void makeResponse();
	void readResponse();

private:
	int socket_; // 클라이언트 소켓
	int resource_fd_[2]; // 0: read, 1: write
	std::string remain_buffer_;
	ClientStatus status_;
	Request request_;
	Response response_;
};
