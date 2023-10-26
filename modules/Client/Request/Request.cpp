#include "Request.hpp"

Request::Request() : status_(START_LINE) {}

void Request::parse(int fd) {
    std::cout << "Reqeust parse" << std::endl;
}