#pragma once

#include <unistd.h>

#include "../Config/ConfigManager.hpp"
#include "../Server/ServerManager.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "UData/UData.hpp"
#include "RequestHandler.hpp"

enum ClientStatus {
    PARSE_REQUEST,
    READ_CGI,
    WRITE_CGI,
    READ_FILE,
    WRITE_FILE,
    SEND_RESPONSE
};

class Client {
public:
    Client(int socket);
    ~Client();

    void handleSocketReadEvent();
    void handleSocketWriteEvent();
    void handleCgiReadEvent();
    void handleCgiWriteEvent();
    void handleCgiTimeoutEvent();
    void handleFileReadEvent();
    void handleFileWriteEvent();

    ClientStatus status_;
    int socket_;
    int resource_fd_[2];
    pid_t pid_;
    Request request_;
    Response response_;
    ServerBlock* server_;
    LocationBlock* location_;
};