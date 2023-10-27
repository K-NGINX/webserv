#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unistd.h>

#include "../../Utils/Utils.hpp"

#define BUFFER_SIZE 1024
#define VERSION "HTTP/1.1"
#define PLAIN_TEXT "text/plain"
#define HTML_TEXT "text/html"
#define JSON_TEXT "application/json"

enum RequestStatus {
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
	void parseStartLine(std::vector<char>& line);
	void parseHeader(std::vector<char>& line);
	void parseBody(std::vector<char>& line);
	void checkReadSize();

    RequestStatus parsing_status_;
	std::vector<char> remain_buffer_;
	std::string method_;
	std::string uri_;
	std::string host_;
	std::map<std::string, std::string> m_header_;
	std::string body_; // 여기에 body 저장 append로 붙히명 \0 노상관
};
