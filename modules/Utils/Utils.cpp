#include "Utils.hpp"

std::string Utils::whitespace = " \r\n\t\v\f";
// MIME
std::map<std::string, std::string> initializeMIMEMap() {
	std::map<std::string, std::string> m_mime;
	m_mime["html"] = "text/html";
	m_mime["txt"] = "text/plain";
	m_mime["png"] = "image/png";
	m_mime["ico"] = "image/x-icon";
	m_mime["binary"] = "multipart/form-data";
	return m_mime;
}
std::map<std::string, std::string> Utils::m_mime = initializeMIMEMap();

Utils::Utils() {}
Utils::~Utils() {}

void Utils::trimComment(std::string& str) {
	size_t pos_comment = str.find_first_of('#');
	if (pos_comment != std::string::npos) {
		str.erase(pos_comment);
	}
}

void Utils::trimWhiteSpace(std::string& str) {
	size_t pos_start = str.find_first_not_of(whitespace);
	if (pos_start != std::string::npos) {
		str.erase(0, pos_start);
	}
	size_t pos_end = str.find_last_not_of(whitespace);
	if (pos_end != std::string::npos) {
		str.erase(pos_end + 1);
	}
}

std::string Utils::getMIMEType(const std::string& file_type) {
	if (m_mime.find(file_type) != m_mime.end())
		return m_mime[file_type];
	else
		return "";
}

bool Utils::checkMIMEType(const std::string& mime_type) {
	std::map<std::string, std::string>::iterator mime_it = m_mime.begin();
	while (mime_it != m_mime.end()) {
		if (mime_it->second == mime_type)
			return true;
		mime_it++;
	}
	return false;
}

int Utils::stoi(std::string str) {
	return atoi(str.c_str());
}
/**
 * @brief v에서 obj를 찾아서 obj의 끝 인덱스를 반환해주는 함수
 *
 * @param v base vector
 * @param obj target vector
 * @return int 찾은 sub obj의 끝 인덱스를 반환
 */
int Utils::findSubVector(std::vector<char>& v, std::vector<char>& obj) {
	for (size_t i = 0; i < v.size(); ++i) {
		bool found = true;

		for (size_t j = 0; j < obj.size(); ++j) {
			if (i + j >= v.size() || v[i + j] != obj[j]) {
				found = false;
				break;
			}
		}
		if (found)
			return static_cast<int>(i + obj.size() - 1); // obj의 마지막 원소의 인덱스 반환;
	}

	return -1; // obj가 v에 없음
}

ssize_t Utils::hexToDecimal(const std::string& hex_string) {
	ssize_t result = 0;

	for (size_t i = 0; i < hex_string.size(); i++) {
		result *= 16; // 16진수이므로 16을 곱해줍니다.

		if (hex_string[i] >= '0' && hex_string[i] <= '9') {
			result += hex_string[i] - '0'; // '0' ~ '9'까지의 경우
		} else if (hex_string[i] >= 'a' && hex_string[i] <= 'f') {
			result += hex_string[i] - 'a' + 10; // 'a' ~ 'f'까지의 경우
		} else if (hex_string[i] >= 'A' && hex_string[i] <= 'F') {
			result += hex_string[i] - 'A' + 10; // 'A' ~ 'F'까지의 경우
		} else {
			// 잘못된 16진수 문자가 들어온 경우 처리
			std::cerr << "Error: Invalid hex character - " << hex_string[i] << std::endl;
			return -1; // 에러를 나타내는 값을 반환
		}
	}

	return result;
}
