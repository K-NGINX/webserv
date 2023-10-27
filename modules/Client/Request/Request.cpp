#include "Request.hpp"

Request::Request() : parsing_status_(START_LINE) {}

void Request::parse(int fd) {
    (void)fd;
}
