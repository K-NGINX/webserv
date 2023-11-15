#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../../Utils/Utils.hpp"

#define VERSION "HTTP/1.1"

class Response {
   public:
	Response();
	Response& operator=(const Response& obj);
	void print();

	void makeResponse(bool is_keep_alive);
	void setContentType(const std::string& resource);
	//gettersetter
	const std::vector<char>& getSendBuffer() const;
	void setBody(const std::vector<char>& obj);
	void setStatusCode(const std::string& obj);

   private:
	std::vector<char> send_buffer_;
	std::string status_code_;
	std::string content_type_;
	std::map<std::string, std::string> m_header_;
	std::vector<char> body_;
	
	std::vector<char> getStatusLine() const;
	void makeHeaderLine(bool is_keep_alive);
	std::string getResponseDate(std::time_t* t);
};

template <typename T>
	std::string ntos(T n) {
	std::stringstream ss;
	ss << n;
	return ss.str();
}