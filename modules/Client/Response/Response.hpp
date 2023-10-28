#pragma once

#include <iostream>
#include <map>
#include <vector>

class Response {
   public:
	Response();

	std::string status_code_;
	std::map<std::string, std::string> m_header_;
	std::vector<char> body_;
};
