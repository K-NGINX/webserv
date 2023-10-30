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
	void clear();

	std::string status_code_;
	std::map<std::string, std::string> m_header_;
	std::vector<char> body_;

	void makeResponse(std::vector<char> &msg);

	private:
		std::vector<char> getStatusLine(const std::string &status_code_) const;
		void makeHeaderLine();
		std::string getResponseDate(std::time_t *t);
};

template <typename T>
std::string ntos(T number) { //히익
  std::stringstream ss;
  ss << number;
  return ss.str();
}
