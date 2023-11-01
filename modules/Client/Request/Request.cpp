#include "Request.hpp"

Request::Request() : parsing_status_(INIT), body_size_(0), is_chunked(false), is_chunked_body_end(false) {}

Request& Request::operator=(const Request& obj) {
	parsing_status_ = obj.parsing_status_;
	remain_buffer_ = obj.remain_buffer_;
	method_ = obj.method_;
	uri_ = obj.uri_;
	host_ = obj.host_;
	m_header_ = obj.m_header_;
	body_ = obj.body_;
	body_size_ = obj.body_size_;
	is_chunked = obj.is_chunked;
	is_chunked_body_end = obj.is_chunked;
	connection_ = obj.connection_;
	return *this;
}

void Request::print() {
	std::cout << GRAY << "\n[ REQUEST ]" << std::endl;
	std::cout << method_ << " " << uri_ << " HTTP/1.1" << std::endl;
	std::cout << "Host: " << host_ << std::endl;
	std::cout << "BodySize: " << body_size_ << std::endl;
	if (parsing_status_ == DONE)
		std::cout << "-> DONE" << RESET << std::endl;
	else if (parsing_status_ == ERROR)
		std::cout << "-> ERROR" << RESET << std::endl;
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
		size_t seperator_idx = buffer.size() - 1;
		for (size_t i = buffer.size() - 2; i >= 0; --i) {
			if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
				seperator_idx = i;
				break;
			}
		}
		for (size_t i = seperator_idx + 2; i < buffer.size(); i++)
			res.push_back(buffer[i]);
		// 원래 버퍼 뒤에 붙어있던 것들은 다 지워준다.
		while (buffer.size() >= seperator_idx + 2)
			buffer.pop_back();
	}
	return res;
}
static bool isValidMethod(std::string& str) {
	if (str == "GET" || str == "POST" || str == "DELETE")
		return true;
	return false;
}

void Request::parseStartLine(std::vector<char> &line) {
	std::string s_line(line.begin(), line.end()), tmp;
	std::stringstream ss(s_line);

	std::vector<std::string> split;
	while (ss >> tmp)
		split.push_back(tmp);

	if (split.size() != 3 || isValidMethod(split[0]) == false || split[2] != VERSION) {
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
	std::vector<char>::iterator it = std::find(line.begin(), line.end(), ':');
	if (it == line.end()) {
		parsing_status_ = ERROR;
		return;
	}
	// key value로 나눠서 넣어준다.
	std::string key(line.begin(), it);
	std::string value(it + 1, line.end());
	Utils::trimWhiteSpace(value);
	if (value == "") {
		parsing_status_ = ERROR;
		return;
	}
	// body 파일 제한
	if (key == "Content-Type" && Utils::checkMIMEType(value) == false) {
		parsing_status_ = ERROR;
		return;
	}
	if (key == "Transfer-Encoding" && value == "chunked")
		is_chunked = true;
	if (key == "Host")
		host_ = value;
	if (key == "Connection")
		connection_ = value;
	m_header_[key] = value;
}

void Request::parseBody(std::vector<char> &line) {
	if (line.size() == 0) {
		parsing_status_ = ERROR;
		return;
	}
	body_size_ += line.size();
	line.push_back('\r');
	line.push_back('\n');
	for (size_t i = 0; i < line.size(); i++)
		body_.push_back(line[i]);
}
/**
 * @brief 모든 request를 읽었을때 호출되며 request 형식에 부합하는지 마지막으로 검증하는 함수
 *
 */
void Request::checkValidRequest() {
	if (host_ == "" || remain_buffer_.size() != 0 || parsing_status_ != BODY || (is_chunked && is_chunked_body_end == false)) {	   // chunked인데 end가 아니거나
		parsing_status_ = ERROR;
		return;
	}
	// post일때 유효한 request일 경우
	if (method_ == "POST") {
		if (m_header_.find("content-length") != m_header_.end() && body_size_ != Utils::stoi(m_header_["content-length"])) {
			parsing_status_ = ERROR;
			return;
		}
		parsing_status_ = DONE;
		return;
	} else if ((method_ == "GET" || method_ == "DELETE") && body_size_ == 0) {
		parsing_status_ = DONE;
		return;
	}
	parsing_status_ = ERROR;
}

void Request::parseChunkedBody(std::vector<char> &size, std::vector<char> &line) {
	std::string s_size(size.begin(), size.end());
	size_t line_size = Utils::stoi(s_size);
	if (line_size != line.size()) {
		parsing_status_ = ERROR;
		return;
	}
	line.push_back('\r');
	line.push_back('\n');
	body_size_ += line_size;
	for (size_t i = 0; i < line.size(); i++)
		body_.push_back(line[i]);
	if (body_size_ == 0)
		is_chunked_body_end = true;
}

void Request::parse(int fd) {
	// (void)fd;
	// char buffer[BUFFER_SIZE] = "POST /index.html HTTP/1.1\r\nHost:www.example.com\r\nTransfer-Encoding:chunked\r\n\r\n4\r\nbody\r\n4\r\nbody\r\n";
	//   char buffer[BUFFER_SIZE] = "GET /index.html HTTP/1.1\r\nHost:dfdfd\r\n\r\n";
	char buffer[BUFFER_SIZE];
	int read_size = read(fd, buffer, BUFFER_SIZE);
	if (parsing_status_ == INIT && read_size <= 0)	  // 클라이언트와 연결되어 있지만, 요청을 받은 상태는 아님
		return;
	if (parsing_status_ == INIT)
		parsing_status_ = START_LINE;
	if (read_size == 0)	// EOF
		checkValidRequest();
	if (read_size == -1)
		parsing_status_ = ERROR;
	if (parsing_status_ == ERROR || parsing_status_ == DONE)
		return;
	// remainbuf에 이어붙힌다.
	for (int i = 0; i < read_size; i++)
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
				if (is_chunked) {
					if (is_chunked_body_end == true || i + 1 >= v_splited_line.size()) {
						parsing_status_ = ERROR;
						return;
					}
					parseChunkedBody(v_splited_line[i], v_splited_line[i + 1]);
					i++;
				} else
					parseBody(v_splited_line[i]);
				break;
			default:
				return;
		}
	}
	if (parsing_status_ == BODY && remain_buffer_.size() == 0)
		checkValidRequest();
}

// int main() {
//   Request req;
//   req.parse(0);

//   req.checkValidRequest();
//   req.print();
// }
