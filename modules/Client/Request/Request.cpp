#include "Request.hpp"

#include "errno.h"

Request::Request() : parsing_status_(START_LINE), host_("no_host"), body_size_(0) {
}

void Request::clear() {
	
}

void Request::print() {
	std::cout << "[ REQUEST ]" << std::endl;
	std::cout << method_ << " " << uri_ << " HTTP/1.1" << std::endl;
	std::cout << "Host:" << host_ << std::endl;
	std::cout << "BodySize:" << body_size_ << std::endl;
	if (parsing_status_ == DONE)
		std::cout << "-> DONE" << std::endl;
	else if (parsing_status_ == ERROR)
		std::cout << "-> ERROR" << std::endl;
}

static std::vector<std::vector<char> > splitVector(std::vector<char> &line) {
	std::vector<std::vector<char> > res;
	std::vector<char> temp;

	for (size_t i = 0; i < line.size(); ++i) {
		if (line[i] == '\r' && i < line.size() - 1 && line[i + 1] == '\n') {
			res.push_back(temp);
			temp.clear();
			i++;
		} else {
			temp.push_back(line[i]);
		}
	}
	if (!temp.empty())
		res.push_back(temp);
	return res;
}

static std::vector<char> getRemainBuffer(std::vector<char> &buffer) {
	std::vector<char> res;
	size_t last_idx = buffer.size() - 1;
	if (buffer[last_idx] != '\n' || buffer[last_idx - 1] != '\r') {
		size_t start_remain_unit_idx = buffer.size() - 1;
		for (size_t i = buffer.size() - 2; i >= 0; --i) {
			if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
				start_remain_unit_idx = i;
				break;
			}
		}
		for (size_t i = start_remain_unit_idx + 2; i < buffer.size(); i++)
			res.push_back(buffer[i]);
		// 원래 버퍼 뒤에 붙어있던 것들은 다 지워준다.
		while (buffer.size() >= start_remain_unit_idx + 2)
			buffer.pop_back();
	}
	return res;
}

void Request::parseStartLine(std::vector<char> &line) {
	std::string s_line(line.begin(), line.end()), tmp;
	std::stringstream ss(s_line);

	std::vector<std::string> split;
	while (ss >> tmp)
		split.push_back(tmp);

	if (split.size() != 3 || (split[0] != "GET" && split[0] != "POST" && split[0] != "DELETE") || split[2] != VERSION) {
		parsing_status_ = ERROR;
		return;
	}
	method_ = split[0];
	uri_ = split[1];
	parsing_status_ = HEADER;
}

void Request::parseHeader(std::vector<char> &line) {
	if (line.size() == 0) {
		parsing_status_ = BODY;
		return;
	}
	size_t sep_idx = std::find(line.begin(), line.end(), ':') - line.begin();
	if (sep_idx == line.size()) {
		parsing_status_ = ERROR;
		return;
	}
	// key value로 나눠서 넣어준다.
	std::string key(line.begin(), line.begin() + sep_idx);
	std::string value(line.begin() + sep_idx + 1, line.end());
	Utils::trimWhiteSpace(value);
	// body 파일 제한
	if (key == "Content-Type" && value != PLAIN_TEXT && value != HTML_TEXT && value != JSON_TEXT) {
		parsing_status_ = ERROR;
		return;
	}
	if (key == "Host")
		host_ = value;
	m_header_[key] = value;
}

void Request::parseBody(std::vector<char> &line) {
	if (line.size() == 0) {
		parsing_status_ = ERROR;
		return;
	}
	if (m_header_.find("transfer-encoding") != m_header_.end() && m_header_["transfer-encoding"] == "chunked") {
		// chunked body parsing
		// chunked size가 -1이면 아직 size를 못읽었다는 뜻
		return;
	}

	// for (size_t i = 0; i < line.size(); i++)
	// 	std::cout << line[i];
	// std::cout << std::endl;

	body_size_ += line.size();
	line.push_back('\r');
	line.push_back('\n');
	for (size_t i = 0; i < line.size(); i++)
		body_.push_back(line[i]);
}

void Request::checkValidRequest() {
	if (host_ == "no_host") {
		parsing_status_ = ERROR;
		return;
	}
	if (remain_buffer_.size() != 0 || parsing_status_ != BODY) {
		parsing_status_ = ERROR;
		return;
	}
	// post일때 유효한 request일 경우
	if (method_ == "POST") {
		if (m_header_.find("content-length") != m_header_.end() &&
			body_size_ == Utils::stoi(m_header_["content-length"])) {
			parsing_status_ = DONE;
			return;
		}
	} else if ((method_ == "GET" || method_ == "DELETE") && body_size_ == 0) {
		parsing_status_ = DONE;
		return;
	}
	parsing_status_ = ERROR;
}

void Request::parse(int fd) {
	//   char buffer[BUFFER_SIZE] = "POST /index.html
	//   HTTP/1.1\r\nHost:www.example.com\r\ncontent-length:8\r\n\r\nbody\r\nbody\r\n\r\n";
	//   char buffer[BUFFER_SIZE] =
	//       "GET /index.html HTTP/1.1\r\nHost:dfdfd\r\n\r\n";
	// 	  (void)fd;
	char buffer[BUFFER_SIZE];
	int read_size = read(fd, buffer, BUFFER_SIZE);
	if (read_size == 0)	   // EOF
		checkValidRequest();
	if (read_size == -1)
		parsing_status_ = ERROR;
	if (parsing_status_ == ERROR || parsing_status_ == DONE)
		return;
	// remainbuf에 이어붙힌다.
	for (size_t i = 0; buffer[i]; i++)
		remain_buffer_.push_back(buffer[i]);

	std::vector<char> new_remain_buffer = getRemainBuffer(remain_buffer_);
	std::vector<std::vector<char> > v_splited_line = splitVector(remain_buffer_);
	remain_buffer_ = new_remain_buffer;

	for (size_t i = 0; i < v_splited_line.size(); i++) {
		switch (parsing_status_) {
			case START_LINE:
				parseStartLine(v_splited_line[i]);
				break;
			case HEADER:
				parseHeader(v_splited_line[i]);
				break;
			case BODY:
				// chunked면 유형 맞는지 체크
				parseBody(v_splited_line[i]);
				break;
			default:
				return;
		}
	}
}

// int main() {
//   Request req;
//   req.parse(0);

//   req.checkValidRequest();
//   printRequest(req);
// }
