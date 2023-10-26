#include "Client.hpp"

Client::Client(int socket) : socket_(socket), pid_(-1), status_(PARSE_REQUEST) {
    resource_fd_[0] = -1;
    resource_fd_[1] = -1;
}

Client::~Client() {
    close(socket_);
    close(resource_fd_[0]);
    close(resource_fd_[1]);
}

void Client::handleSocketReadEvent() { // request가 왔다
    if (status_ != PARSE_REQUEST)
        return;
    request_.parse(socket_);
    if (request_.status_ == DONE || request_.status_ == ERROR) {
        ServerManager::getInstance().kqueue_.unregisterReadEvent(socket_);
        HandleRequest(*this);
    }
}

void Client::handleSocketWriteEvent() { // response 보낼 수 있다
    if (status_ != SEND_RESPONSE)
        return;
    // start line 세팅
    // header 세팅
    // response 전문 생성
    // socket에 response 쓰기
    // socket 제거, close
}

void Client::handleCgiReadEvent() {
    if (status_ != READ_CGI)
        return;
    std::cout << "handleCgiReadEvent" << std::endl;
    // waitpid
    // 다 읽었다면 반환값 response 헤더 파싱
    // 읽기 실패 -> 500
    // 양방향 파이프 resource_fd 닫기
    status_ = SEND_RESPONSE;
}

void Client::handleCgiWriteEvent() {
    if (status_ != WRITE_CGI)
        return;
    std::cout << "handleCgiWriteEvent" << std::endl;
    // request body fd에 쓰기
    status_ = SEND_RESPONSE;
}

void Client::handleCgiTimeoutEvent() {
    std::cout << "handleCgiTimeoutEvent" << std::endl;
}

void Client::handleFileReadEvent() {
    if (status_ != READ_FILE)
        return;
    std::cout << "handleFileReadEvent" << std::endl;
    // 파일 크기만큼 response 본문에 저장
    // 읽기 실패 -> 500
    status_ = SEND_RESPONSE;
}

void Client::handleFileWriteEvent() {
    if (status_ != WRITE_FILE)
        return;
    std::cout << "handleFileWriteEvent" << std::endl;
    // request body fd에 쓰기
    status_ = SEND_RESPONSE;
}
