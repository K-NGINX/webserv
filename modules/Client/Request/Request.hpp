#pragma once

#include <iostream>

enum ParsingRequestStatus {
    START_LINE,
    HEADER,
    EMPTY_LINE,
    BODY,
    DONE,
    ERROR
};

class Request {
public:
    Request();

    void parse(int fd);

    ParsingRequestStatus status_;
};