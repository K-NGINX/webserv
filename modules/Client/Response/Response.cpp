#include "Response.hpp"

Response::Response()
	: status_code_("200") {}

const std::vector<char>& Response::getStatusMessage(const std::string& status_code_) const {
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

	std::string status_message = m_status[status_code_];
	return std::vector<char>(status_message.begin(), status_message.end());
}