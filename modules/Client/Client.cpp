#include "Client.hpp"

#include "RequestHandler/RequestHandler.hpp"

Client::Client(int socket) : status_(RECV_REQUEST),
							 socket_(socket),
							 is_keep_alive_(true),
							 pid_(-1),
							 server_(NULL),
							 location_(NULL),
							 written_(0),
							 cgi_written_(0) {
	pipe_fd_[0] = -1;
	pipe_fd_[1] = -1;
}

Client::~Client() { close(socket_); }

/**
 * @brief keep-alive 옵션을 위해 클라이언트의 연결을 끊지 않고 다시 요청을 받을 수 있는 상태로 만들어줌
 *
 */
void Client::clear() {
	status_ = RECV_REQUEST;
	is_keep_alive_ = true;
	pid_ = -1;
	pipe_fd_[0] = -1;
	pipe_fd_[1] = -1;
	request_ = Request();
	response_ = Response();
	server_ = NULL;
	location_ = NULL;
	written_ = 0;
	cgi_written_ = 0;
	ServerManager::getInstance().kqueue_.startMonitoringReadEvent(socket_, this);
}

void Client::setStatus(const ClientStatus& status) {
	status_ = status;
}

/**
 * @brief 클라이언트로부터 온 Request를 소켓으로부터 읽어 파싱하는 함수
 *
 */
void Client::handleSocketReadEvent() {
	char read_buffer[BUFFER_SIZE];
	int read_size = read(socket_, read_buffer, BUFFER_SIZE);

	request_.parse(read_buffer, read_size);
	if (request_.getParsingStatus() == DONE || request_.getParsingStatus() == ERROR) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(socket_);
		RequestHandler::handleRequest(*this);
	}
}

/**
 * @brief 클라이언트에게 보낼 Response를 소켓에 쓰는 함수
 *
 */
void Client::handleSocketWriteEvent() {
	const std::vector<char>& send_buffer = response_.getSendBuffer();
	ssize_t write_size = send_buffer.size() - written_ > BUFFER_SIZE ? BUFFER_SIZE : send_buffer.size() - written_;
	write_size = write(socket_, &send_buffer[written_], write_size);
	if (write_size == -1) {
		status_ = WILL_DISCONNECT;
		return;
	}
	written_ += write_size;
	if (written_ == static_cast<ssize_t>(send_buffer.size()))
		status_ = WILL_DISCONNECT;
}

/**
 * @brief CGI 프로그램의 결과를 파이프로부터 읽어 파싱하는 함수
 *
 */
void Client::handleCgiReadEvent() {
	int exit_status;
	waitpid(pid_, &exit_status, 0);
	if (WIFSIGNALED(exit_status)) { // 시그널에 의한 종료
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(pipe_fd_[0]);
		close(pipe_fd_[0]);
		RequestHandler::handleError(*this, "500");
		return;
	}

	char buffer[BUFFER_SIZE];
	ssize_t read_size = read(pipe_fd_[0], buffer, BUFFER_SIZE);
	if (read_size == -1) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(pipe_fd_[0]);
		close(pipe_fd_[0]);
		RequestHandler::handleError(*this, "500");
		return;
	}
	response_.pushBackSendBuffer(buffer, read_size);
	if (read_size == 0) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(pipe_fd_[0]);
		close(pipe_fd_[0]);
		ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(socket_, this);
		status_ = SEND_RESPONSE;
	}
}

/**
 * @brief CGI 프로그램을 실행하기 위한 데이터를 파이프에 쓰는 함수
 *
 */
void Client::handleCgiWriteEvent() {
	const std::vector<char>& body = request_.getBody();
	ssize_t write_size = body.size() - cgi_written_ > BUFFER_SIZE ? BUFFER_SIZE : body.size() - cgi_written_;
	write_size = write(pipe_fd_[1], &body[cgi_written_], write_size);
	if (write_size == -1) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(pipe_fd_[0]);
		ServerManager::getInstance().kqueue_.stopMonitoringWriteEvent(pipe_fd_[1]);
		close(pipe_fd_[0]);
		close(pipe_fd_[1]);
		RequestHandler::handleError(*this, "500");
		return;
	}
	cgi_written_ += write_size;
	if (cgi_written_ == static_cast<ssize_t>(body.size())) {
		ServerManager::getInstance().kqueue_.stopMonitoringWriteEvent(pipe_fd_[1]);
		close(pipe_fd_[1]);
		status_ = READ_CGI;
	}
}

/**
 * @brief 파일의 내용을 파일 디스크립터로부터 읽어 파싱하는 함수
 *
 */
void Client::handleFileReadEvent() {
	char buffer[BUFFER_SIZE];
	ssize_t read_size;
	while ((read_size = read(file_fd_, buffer, BUFFER_SIZE)) > 0)
		response_.pushBackBody(buffer, read_size);
	if (read_size == -1) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(file_fd_);
		close(file_fd_);
		RequestHandler::handleError(*this, "500");
	} else if (read_size == 0) {
		ServerManager::getInstance().kqueue_.stopMonitoringReadEvent(file_fd_);
		close(file_fd_);
		response_.makeResponse(is_keep_alive_);
		ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(socket_, this);
		status_ = SEND_RESPONSE;
	}
}
