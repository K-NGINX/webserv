#include "Response.hpp"

Response::Response() : status_code_("200") {}

void Response::clear() {
	status_code_ = "200";
	m_header_.clear();
	body_.clear();
}
void Response::makeResponse(std::vector<char> &msg) {
	std::vector<char> status_line = getStatusLine(status_code_);
	std::vector<char> rn;
	rn.push_back('\r');
	rn.push_back('\n');
	std::string header;
	makeHeaderLine();
	std::map<std::string, std::string>::const_iterator it = this->m_header_.begin();

	msg.insert(msg.end(), status_line.begin(), status_line.end());

	for (; it != m_header_.end(); ++it)
		header += it->first + ": " + it->second + "\r\n";
	msg.insert(msg.end(), header.begin(), header.end());
	msg.insert(msg.end(), rn.begin(), rn.end());
	msg.insert(msg.end(), body_.begin(), body_.end());
}

std::vector<char> Response::getStatusLine(const std::string &status_code_) const {
	static std::map<std::string, std::string> m_status;
	// 한번만 실행됨 !
	if (m_status.empty()) {
		m_status["200"] = "OK";
		m_status["201"] = "Created";
		m_status["204"] = "No Content";
		m_status["301"] = "Moved Permanetly";
		m_status["400"] = "Bad Request";
		m_status["404"] = "Not Found";
		m_status["405"] = "Method Not Allowed";
		m_status["413"] = "Payload Too Large";
		m_status["500"] = "Internal Server Error";
	}

	std::string status_line = status_code_ + " " + m_status[status_code_] + " " + VERSION + "\r\n";
	return std::vector<char>(status_line.begin(), status_line.end());
}

void Response::makeHeaderLine() {
	m_header_["Date"] = getResponseDate(NULL);
	if (body_.size() != 0)
		m_header_["Content-length"] = ntos(body_.size());
}

std::string Response::getResponseDate(std::time_t *t) {
	char buffer[128];
	std::time_t current_time;

	if (t == NULL)
		current_time = std::time(NULL);
	else
		current_time = *t;
	std::tm *timeInfo = std::gmtime(&current_time);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

	return buffer;
}
