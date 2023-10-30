#pragma once

#include <iostream>
#include <map>
#include <vector>

#define VERSION "HTTP/1.1"

class Response {
   public:
	Response();

	std::string status_code_;
	std::map<std::string, std::string> m_header_;
	std::vector<char> body_;

	private:
		std::vector<char> getStatusLine(const std::string& status_code_) const;
};
