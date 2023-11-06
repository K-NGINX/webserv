#pragma once

#include <unistd.h>

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../../Utils/Utils.hpp"

#define BUFFER_SIZE 1024
#define VERSION "HTTP/1.1"
// #define PLAIN_TEXT "text/plain"
// #define HTML_TEXT "text/html"
// #define JSON_TEXT "application/json"

enum RequestStatus {
	INIT,
	START_LINE,
	HEADER,
	BODY,
	DONE,
	ERROR
};

class Request {
   public:
	Request();
	Request& operator=(const Request& obj);

	void parse(int fd);
	void parseStartLine(std::vector<char> &line);
	void parseHeader(std::vector<char> &line);
	void parseBody(std::vector<char> &line);
	void parseChunkedBody(std::vector<char> &size, std::vector<char> &line);
	void checkValidRequest();
	void print();
 // gettersetter
	std::string getConnection() const;
	RequestStatus getParsing_status() const;
	std::string getMethod() const;
	std::string getUri() const;
	std::string getHost() const;
	int getBodySize();
   private:
	std::string connection_;
	RequestStatus parsing_status_;
	std::string method_;
	std::string uri_;
	std::string host_;
	std::vector<char> remain_buffer_;
	std::map<std::string, std::string> m_header_; ////////////////////
	std::vector<char> body_;
	int body_size_;
	bool is_chunked;
	bool is_chunked_body_end;
};
