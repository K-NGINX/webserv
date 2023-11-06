#include "Response.hpp"

Response::Response() : status_code_("200") {}

Response &Response::operator=(const Response &obj) {
	status_code_ = obj.status_code_;
	content_type_ = obj.content_type_;
	m_header_ = obj.m_header_;
	body_ = obj.body_;
	return *this;
}

void Response::setBody(const std::vector<char>& obj) { body_ = obj; }
void Response::setStatusCode(const std::string& obj) { status_code_ = obj; }

void Response::print() {
	std::cout << GRAY << "\n[ RESPONSE ]" << std::endl;
	std::map<std::string, std::string>::iterator header_it = m_header_.begin();
	std::vector<char> status_line = getStatusLine();
	std::cout << std::string(status_line.begin(), status_line.end());
	while (header_it != m_header_.end()) {
		std::cout << header_it->first << ": " << header_it->second << std::endl;
		header_it++;
	}
	std::cout << RESET << std::endl;
}

void Response::setContentType(const std::string &resource) {
	std::string file_type = resource.substr(resource.find('.') + 1);
	// Request에서 파일 타입 제한을 이미 했기 때문에 여기로 흘러온 이상 무조건 있다고 판단
	content_type_ = Utils::getMIMEType(file_type);
}

void Response::makeResponse(std::vector<char> &msg, bool is_keep_alive) {
	std::vector<char> status_line = getStatusLine();
	std::vector<char> rn;
	rn.push_back('\r');
	rn.push_back('\n');
	std::string header;
	makeHeaderLine(is_keep_alive);
	std::map<std::string, std::string>::const_iterator it = this->m_header_.begin();

	msg.insert(msg.end(), status_line.begin(), status_line.end());

	for (; it != m_header_.end(); ++it)
		header += it->first + ": " + it->second + "\r\n";
	msg.insert(msg.end(), header.begin(), header.end());
	msg.insert(msg.end(), rn.begin(), rn.end());
	msg.insert(msg.end(), body_.begin(), body_.end());

	print();
}

std::vector<char> Response::getStatusLine() const {
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

	std::string status_line = std::string(VERSION) + " " + status_code_ + " " + m_status[status_code_] + "\r\n";
	return std::vector<char>(status_line.begin(), status_line.end());
}

void Response::makeHeaderLine(bool is_keep_alive) {
	m_header_["Date"] = getResponseDate(NULL);
	if (body_.size() != 0)
		m_header_["Content-Length"] = ntos(body_.size());
	m_header_["Content-Type"] = content_type_;
	m_header_["Connection"] = is_keep_alive == true ? "keep-alive" : "Closed";
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
