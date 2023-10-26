#pragma once

enum ParsingRequestStatus {
	START_LINE,
	HEADER,
	BODY,
	ERROR_PARSING // error면 파싱안하고
};

class Request {
public:
    Request();

    void parse(int fd);

    ParsingRequestStatus parsing_status_;
};