#include "Client.hpp"

#include "RequestHandler/RequestHandler.hpp"

Client::Client(int socket)
	: status_(RECV_REQUEST), socket_(socket), is_keep_alive_(true), pid_(-1), server_(NULL), location_(NULL) {}

Client::~Client() { close(socket_); }

/**
 * @brief keep-alive 옵션을 위해 클라이언트의 연결을 끊지 않고 다시 요청을 받을 수 있는 상태로 만들어줌
 * 
 */
void Client::clearStatus() {
	status_ = RECV_REQUEST;
	pid_ = -1;
	server_ = NULL;
	location_ = NULL;
	ServerManager::getInstance().kqueue_.registerReadEvent(socket_, this);
}

void Client::willDisconnect() {
	status_ = WILL_DISCONNECT;
}

void Client::handleSocketReadEvent() {	  // request가 왔다
	request_.parse(socket_);
	if (request_.parsing_status_ == DONE || request_.parsing_status_ == ERROR) {
		ServerManager::getInstance().kqueue_.unregisterReadEvent(socket_);	  // 클라이언트 소켓에 대한 read 이벤트를 더이상 감시하지 않겠다 !
		RequestHandler::handleRequest(*this);
	}
}

void Client::handleSocketWriteEvent() {	   // response 보낼 수 있다
										   // start line 세팅
										   // header 세팅
										   // response 전문 생성
										   // socket에 response 쓰기
										   // status_ = WILL_DISCONNECT;
}

void Client::handleCgiReadEvent(int fd) {
	std::cout << "handleCgiReadEvent" << std::endl;
	(void)fd;
	// waitpid
	// 다 읽었다면 반환값 response 헤더 파싱
	// 읽기 실패 -> 500
	// 양방향 파이프 resource_fd 닫기
	status_ = SEND_RESPONSE;
}

void Client::handleCgiWriteEvent(int fd) {
	std::cout << "handleCgiWriteEvent" << std::endl;
	(void)fd;
	// request body fd에 쓰기
	status_ = READ_CGI;
}

void Client::handleFileReadEvent(int fd) {
	std::cout << "handleFileReadEvent" << std::endl;
	// 파일 크기만큼 response 본문에 저장
	// 읽기 실패 -> 500
	int read_size = 0;
	char buffer[BUFFER_SIZE];
	while (true) {
		read_size = read(fd, buffer, BUFFER_SIZE);
		if (read_size == -1) {
			this->response_.status_code_ = "500";
			return;
		} else if (read_size == 0)
			break;
		for (int i = 0; i < read_size; i++)
			this->response_.body_.push_back(buffer[i]);
	}
	for (size_t i = 0; i < response_.body_.size(); i++)
		std::cout << response_.body_[i];
	std::cout << std::endl;
	status_ = SEND_RESPONSE;
}

void Client::handleFileWriteEvent(int fd) {
	std::cout << "handleFileWriteEvent" << std::endl;
	(void)fd;
	// request body fd에 쓰기
	status_ = SEND_RESPONSE;
}
