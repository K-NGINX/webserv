#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../Utils/Utils.hpp"

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
	Request& operator=(const Request& obj);

	/* getter setter */
	const std::string& getConnection() const;
	const RequestStatus& getParsingStatus() const;
	const std::string& getMethod() const;
	const std::string& getUri() const;
	const std::string& getHost() const;
	const std::vector<char>& getBody() const;
	int getBodySize();
	const std::string getContentLength() const;
	const std::string& getBoundary() const;

	void parse(char* read_buffer, int read_size);
	void print();

   private:
	v_c_iter getNextSepIter(v_c_iter line_start_it);

	void parseStartLine(std::vector<char>& line);
	void parseHeader(std::vector<char>& line);
	void parseBody(std::vector<char>& line);
	void parseChunkedBody(std::vector<char>& line, v_c_iter& line_start_it, v_c_iter& next_sep_it);
	void parseBinaryBody(std::vector<char>& line);
	void refineContentType(std::string& value);

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
