#include "Client.hpp"

#include <cstdlib>

#include "RequestHandler/RequestHandler.hpp"

Client::Client(int socket)
	: status_(RECV_REQUEST), socket_(socket), is_keep_alive_(true), pid_(-1), server_(NULL), location_(NULL), written_(0), cgi_write_size_(0) {}

Client::~Client() { close(socket_); }

/**
 * @brief keep-alive 옵션을 위해 클라이언트의 연결을 끊지 않고 다시 요청을 받을 수 있는 상태로 만들어줌
 *
 */
void Client::clear() {
	status_ = RECV_REQUEST;
	is_keep_alive_ = true;
	pid_ = -1;
	request_ = Request();
	response_ = Response();
	server_ = NULL;
	location_ = NULL;
	written_ = 0;
	cgi_write_size_ = 0;
	ServerManager::getInstance().kqueue_.startMonitoringReadEvent(socket_, this);
	ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(socket_, this);
}

void Client::setStatus(const ClientStatus& status) {
	status_ = status;
}

void Client::handleSocketReadEvent() {	  // request가 왔다
	char read_buffer[BUFFER_SIZE];
	int read_size = read(socket_, read_buffer, BUFFER_SIZE);

	request_.parse(read_buffer, read_size);
	if (request_.getParsing_status() == DONE || request_.getParsing_status() == ERROR) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(socket_);	  // 클라이언트 소켓에 대한 read 이벤트를 더이상 감시하지 않겠다 !
		RequestHandler::handleRequest(*this);
	}
}

// response 보낼 수 있다
// start line 세팅
// header 세팅
// response 전문 생성
// socket에 response 쓰기
void Client::handleSocketWriteEvent() {
	std::cerr << "handleSocketWriteEvent" << std::endl;
	std::vector<char> msg;
	response_.makeResponse(msg, is_keep_alive_);
	ssize_t cnt = msg.size() - written_;
	cnt = write(socket_, msg.data() + written_, cnt);
	written_ += cnt;
	// if (cnt == 0)
	//	std::cout << "alsejfailwjfleaiwj iflejwalifj ilawefjil!!\n";
	// if (cnt == 0 || cnt == -1)
	//	std::cout << "error !!!\n";
	status_ = WILL_DISCONNECT;
}

void Client::handleCgiReadEvent(int fd) {
	std::cerr << "handleCgiReadEvent" << std::endl;
	waitpid(pid_, NULL, 0);
	std::cerr << "waitpid after" << std::endl;
	char buffer[BUFFER_SIZE];
	ssize_t read_size = read(fd, buffer, BUFFER_SIZE);
	for (ssize_t i = 0; i < read_size; i++)
		std::cout << buffer[i];
	std::cout << std::endl;
	// waitpid
	// 다 읽었다면 반환값 response 헤더 파싱
	// 읽기 실패 -> 500
	// 양방향 파이프 resource_fd 닫기
	if (read_size <= 0) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(fd);
		close(fd);
		status_ = SEND_RESPONSE;
	}
}

void Client::handleCgiWriteEvent(int fd) {
	const std::vector<char>& body = request_.getBody();
	ssize_t write_size = body.size() - cgi_write_size_ > BUFFER_SIZE ? BUFFER_SIZE : body.size() - cgi_write_size_;
	write_size = write(fd, &body[cgi_write_size_], write_size);
	if (write_size == -1) {	   // 쓰기 실패
		ServerManager::getInstance().kqueue_.stopMonitoringWriteEvent(fd);
		close(fd);
		RequestHandler::handleError(*this, "500");
		return;
	}
	cgi_write_size_ += write_size;			 // 읽은 만큼 더해주기
	if (cgi_write_size_ == static_cast<ssize_t>(body.size())) {	 // 다 썼음
		std::cout << YELLOW << "Done !!!" << RESET << std::endl;
		ServerManager::getInstance().kqueue_.stopMonitoringWriteEvent(fd);
		close(fd);
		status_ = READ_CGI;
	}
}

void Client::handleFileReadEvent(int fd) {
	// 파일 크기만큼 response 본문에 저장
	// 읽기 실패 -> 500
	int read_size = 0;
	char buffer[BUFFER_SIZE];
	std::vector<char> response_body;
	while (true) {
		read_size = read(fd, buffer, BUFFER_SIZE);
		if (read_size == -1)
			return RequestHandler::handleError(*this, "500");
		else if (read_size == 0)
			break;
		for (int i = 0; i < read_size; i++)
			response_body.push_back(buffer[i]);
	}
	response_.setBody(response_body);
	// for (size_t i = 0; i < response_.body_.size(); i++) /////////////////
	// 	std::cout << response_.body_[i];
	// std::cout << std::endl;
	status_ = SEND_RESPONSE;
}

void Client::handleFileWriteEvent(int fd) {
	std::cout << "handleFileWriteEvent" << std::endl;
	(void)fd;
	// request body fd에 쓰기
	status_ = SEND_RESPONSE;
}
