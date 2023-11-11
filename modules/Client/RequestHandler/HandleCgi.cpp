#include "RequestHandler.hpp"

//  - POST 요청이면 WRITE_CGI
//  - CGI 환경 변수 설정
//  - 양방향 파이프 resource_fd 생성
//  - fork()
//  - 자식 : CGI 프로그램 execve
//  - 부모 : waitpid
//  - GET 요청이면 READ_CGI
void RequestHandler::handleCgi(Client &client) {
    const std::vector<char>& body = client.request_.getBody();
    std::cout << GREEN << "===" << std::endl;
    for (size_t i = 0; i < body.size(); i++)
        std::cout << body[i];
    std::cout << "\n===" << RESET << std::endl;
}
