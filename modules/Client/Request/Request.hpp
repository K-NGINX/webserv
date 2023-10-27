#pragma once

#include <iostream>

enum ParsingRequestStatus {
	START_LINE,
	HEADER,
	BODY,
	DONE,
	ERROR // error면 파싱안하고
};

class Request {
public:
    Request();

    void parse(int fd);

    ParsingRequestStatus parsing_status_;
	std::string method_;
	std::string uri_;
	std::string host_;
	std::string body_;
};