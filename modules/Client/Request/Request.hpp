#pragma once

#include <unistd.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../../Utils/Utils.hpp"

#define BUFFER_SIZE 100000
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

enum BodyType {
	DEFAULT,
	CHUNKED,
	BINARY
};

typedef std::vector<char>::iterator v_c_iter;

class Request {
   public:
	Request();
	Request &operator=(const Request &obj);

	void parse(char *read_buffer, int read_size);
	void print();
	// gettersetter
	std::string getConnection() const;
	RequestStatus getParsing_status() const;
	std::string getMethod() const;
	std::string getUri() const;
	std::string getHost() const;
	const std::vector<char> &getBody() const;
	int getBodySize();

   private:
	void parseStartLine(std::vector<char> &line);
	void parseHeader(std::vector<char> &line);
	void parseBody(std::vector<char> &line);
	void parseChunkedBody(std::vector<char> &line, v_c_iter &line_start_it, v_c_iter &next_sep_it);
	void parseBinaryBody(std::vector<char> &line);
	void refineContentType(std::string &value);
	v_c_iter getNextSepIter(v_c_iter line_start_it);
	std::string connection_;
	RequestStatus parsing_status_;
	std::string method_;
	std::string uri_;
	std::string host_;
	std::vector<char> buffer_;
	size_t content_length_;
	std::vector<char> body_;
	size_t body_size_;
	BodyType bodyType_;
	std::string boundary_;
};
