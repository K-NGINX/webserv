#pragma once

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <map>

#include "../Config/Block/Config.hpp"
#include "../Server/ServerManager.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"

enum ClientStatus {
	RECV_REQUEST,
	READ_CGI,
	WRITE_CGI,
	READ_FILE,
	WRITE_FILE,
	SEND_RESPONSE,
	WILL_DISCONNECT
};

class Client {
   public:
	Client(int socket);
	~Client();

	void setStatus(const ClientStatus& status);

	void clear();
	void handleSocketReadEvent();
	void handleSocketWriteEvent();
	void handleCgiReadEvent();
	void handleCgiWriteEvent();
	void handleFileReadEvent();

	Config config_;
	ClientStatus status_;
	int socket_;		 // 클라이언트 소켓: 식별자로 사용됨, 소멸자에서 close
	bool is_keep_alive_; // 기본은 true, 클라이언트 요청에서 Connection: Closed를 받을 때 false
	pid_t pid_;			 // CGI 실행을 위한 자식 프로세스 pid
	int pipe_fd_[2];
	int file_fd_;
	Request request_;
	Response response_;
	const ServerBlock* server_;
	const LocationBlock* location_;
	ssize_t written_;
	ssize_t cgi_written_;
};
