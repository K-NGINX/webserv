#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "../../Utils/Utils.hpp"

#define VERSION "HTTP/1.1"

class Response {
   public:
	Response();
	Response& operator=(const Response& obj);
	void print();

	std::string status_code_;
	std::string content_type_;
	std::map<std::string, std::string> m_header_;
	std::vector<char> body_;

	void makeResponse(std::vector<char> &msg, bool is_keep_alive);
	void setContentType(const std::string& resource);

	private:
		std::vector<char> getStatusLine() const;
		void makeHeaderLine(bool is_keep_alive);
		std::string getResponseDate(std::time_t *t);
};

template <typename T>
std::string ntos(T number) { //히익
  std::stringstream ss;
  ss << number;
  return ss.str();
}
