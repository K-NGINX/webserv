#include "Client.hpp"

Client::Client(int client_socket) : socket_(client_socket), status_(PARSE_REQUEST) {
	resource_fd_[0] = -1;
	resource_fd_[1] = -1;
}

const ClientStatus& Client::getStatus() const { return status_; }

const int& Client::getReadResourceFd() const { return resource_fd_[0]; }

void Client::setStatus(const ClientStatus& status) { status_ = status; }

void Client::parseRequest() {
	char buffer[1024];
	int read_size = read(socket_, buffer,sizeof(buffer));
	if (read_size == -1) {
		// 엌카쥐
	}
	remain_buffer_ += std::string(buffer);
	std::stringstream ss(remain_buffer_);
	std::string line, prev_line;
	// 따로 테스트 해서 올리도록 하겠슴

	// if (request_.getStatus() == DONE || request_.getStatus() == ERROR)
		makeResponse();
}

void Client::makeResponse() {

}

void Client::readResponse() {

}
