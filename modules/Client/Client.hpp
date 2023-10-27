#pragma once

#include <map>
#include <unistd.h>

#include "../Config/ConfigManager.hpp"
#include "../Server/ServerManager.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "RequestHandler.hpp"

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

    void handleSocketReadEvent();
    void handleSocketWriteEvent();
    void handleCgiReadEvent(int fd);
    void handleCgiWriteEvent(int fd);
    void handleFileReadEvent(int fd);
    void handleFileWriteEvent(int fd);
    // void handleCgiTimeoutEvent();

    ClientStatus status_;
    int socket_; // 클라이언트 소켓, 소멸자에서 close
    pid_t pid_; // CGI 실행을 위한 자식 프로세스 pid
    Request request_;
    Response response_;
    ServerBlock* server_;
    LocationBlock* location_;
};
