#pragma once

#include <iostream>

enum ParsingStatus {
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

    ParsingStatus parsing_status_;
};