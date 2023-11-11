#include "Request.hpp"

Request::Request() : parsing_status_(INIT), body_size_(0), bodyType_(DEFAULT) {}

std::string Request::getConnection() const { return connection_; }
RequestStatus Request::getParsing_status() const { return parsing_status_; }
std::string Request::getMethod() const { return method_; }
std::string Request::getUri() const { return uri_; }
std::string Request::getHost() const { return host_; }
const std::vector<char> &Request::getBody() const { return body_; }
int Request::getBodySize() { return body_size_; }

Request &Request::operator=(const Request &obj) {
	parsing_status_ = obj.parsing_status_;
	buffer_ = obj.buffer_;
	method_ = obj.method_;
	uri_ = obj.uri_;
	host_ = obj.host_;
	m_header_ = obj.m_header_;
	body_ = obj.body_;
	body_size_ = obj.body_size_;
	bodyType_ = obj.bodyType_;
	connection_ = obj.connection_;
	return *this;
}

void Request::print() {
	std::cout << GRAY << "\n[ REQUEST ]" << std::endl;
	std::cout << method_ << " " << uri_ << " HTTP/1.1" << std::endl;
	std::cout << "Host: " << host_ << std::endl;
	if (m_header_.find("Content-Length") != m_header_.end())
		std::cout << "Content-Length: " << m_header_["Content-Length"] << std::endl;
	std::cout << "BodySize: " << body_size_ << std::endl;
	if (parsing_status_ == DONE)
		std::cout << "-> DONE" << RESET << std::endl;
	else if (parsing_status_ == ERROR)
		std::cout << "-> ERROR" << RESET << std::endl;
}

static bool isValidMethod(std::string &str) {
	if (str == "GET" || str == "POST" || str == "DELETE")
		return true;
	return false;
}

void Request::parseStartLine(std::vector<char> &line) {
	std::string s_line(line.begin(), line.end()), tmp;
	std::stringstream ss(s_line);
	// std::cout << s_line << std::endl; /////////
	std::vector<std::string> split;
	while (ss >> tmp)
		split.push_back(tmp);

	if (split.size() != 3 || isValidMethod(split[0]) == false || split[2] != VERSION) {
		parsing_status_ = ERROR;
		std::cout << RED << split[2] << RESET << std::endl;
		return;
	}
	method_ = split[0];
	uri_ = split[1];
	parsing_status_ = HEADER;
}

/**
 * @brief content-type value 뒤에 ----boundary_ 잘라주는 함수
 *
 * @param value content-type 뒤에 붙는 ---boundary_ 달려있는 value
 */
void Request::refineContentType(std::string &value) {
	size_t semi_colon = value.find(';');
	if (semi_colon == std::string::npos)
		return;
	size_t equal_oper = value.find('=');
	boundary_ = value.substr(equal_oper);
	bodyType_ = BINARY;
	value.erase(semi_colon);
	std::cout << YELLOW << "boundary : " << boundary_ << "\nnew value : " << value << RESET << std::endl; /////////////
}

void Request::parseHeader(std::vector<char> &line) {
	if (line.size() == 0) {
		if (method_ == "POST")
			parsing_status_ = BODY;
		else {
			buffer_.clear();
			parsing_status_ = DONE;
		}
		if (host_ == "")
			parsing_status_ = ERROR;
		return;
	}
	v_c_iter it = std::find(line.begin(), line.end(), ':');
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
	if (key == "Content-Length" && value != "0" && (method_ == "GET" || method_ == "DELETE")) {
		parsing_status_ = ERROR;
		return;
	}
	if (key == "Content-Type") {
		refineContentType(value);
		if (Utils::checkMIMEType(value) == false) {
			parsing_status_ = ERROR;
			return;
		}
	}
	if (key == "Transfer-Encoding" && value == "chunked")
		bodyType_ = CHUNKED;
	if (key == "Host")
		host_ = value;
	if (key == "Connection")
		connection_ = value;
	m_header_[key] = value;
}

// Content-Type: multipart/form-data; boundary=-----XXX

// -----XXX
// 1111

// -----XXX
// file_name: "kitty.png"

// -----XXX
// 22

// -----XXX--

void Request::parseBody(std::vector<char> &line) {
	size_t content_length = static_cast<size_t>(Utils::stoi(m_header_["Content-Length"]));

	size_t idx = 0;
	while (idx < line.size() && body_size_ < content_length) {
		body_.push_back(line[idx]);
		body_size_++;
		idx++;
	}
	if (body_size_ == content_length)
		parsing_status_ = DONE;
}

/**
 * @brief 모든 request를 읽었을때 호출되며 request 형식에 부합하는지 마지막으로 검증하는 함수
 *
 */
void Request::checkEmptyRemainBuffer() {
	if (buffer_.size() != 0) {	  // chunked인데 end가 아니거나
		parsing_status_ = ERROR;
		return;
	}
}

void Request::parseChunkedBody(std::vector<char> &size, std::vector<char> &line) {
	size_t line_size = Utils::stoi(std::string(size.begin(), size.end()));
	if (line_size != line.size()) {
		parsing_status_ = ERROR;
		return;
	}
	line.push_back('\r');
	line.push_back('\n');
	body_size_ += line_size;
	for (size_t i = 0; i < line.size(); i++)
		body_.push_back(line[i]);
	if (line_size == 0)
		parsing_status_ = DONE;
}

v_c_iter Request::getNextSepIter(v_c_iter &line_start_it) {
	v_c_iter next_sep_it = buffer_.begin();

	while (next_sep_it != buffer_.end()) {
		next_sep_it = std::find(line_start_it, buffer_.end(), '\r');
		if (*(next_sep_it + 1) == '\n')
			break;
	}
	return next_sep_it;
}

void Request::func(std::vector<char>& line, v_c_iter& line_start_it, v_c_iter& next_sep_it) {
	if (next_sep_it == buffer_.end()) { // chunked인데 짝이 안맞음
		parsing_status_ = ERROR;
		return;
	}
	next_sep_it = getNextSepIter(line_start_it);
	std::vector<char> chunked_line(line_start_it, next_sep_it - 1);
	line_start_it = next_sep_it;
	if (next_sep_it != buffer_.end())
		line_start_it += 2;
	parseChunkedBody(line, chunked_line);
}

void Request::parseBinaryBody(std::vector<char> &line) {
	std::vector<char> boundary_end(boundary_.begin(), boundary_.end());
	boundary_end.push_back('-');
	boundary_end.push_back('-');

	int end_idx = Utils::findSubVector(line, boundary_end);
	for (int i = 0; i <= end_idx; i++) {
		body_.push_back(line[i]);
		body_size_++;
	}
	if (end_idx == -1)
		end_idx = line.size() - 1;
	else // body parsing 끝
		parsing_status_ = DONE;
}

void Request::parse(int fd) {
	// (void)fd;
	// char buffer[BUFFER_SIZE] = "POST /index.html HTTP/1.1\r\nHost:www.example.com\r\nContent-Length:24\r\n\r\nbody\r\nbody\r\nbody\r\nbody\r\n";
	//   char buffer[BUFFER_SIZE] = "GET /index.html HTTP/1.1\r\nHost:dfdfd\r\ndfd";
	char read_buffer[BUFFER_SIZE];
	int read_size = read(fd, read_buffer, BUFFER_SIZE);
	// for (int i = 0; i < read_size; i++)
	// 	std::cout << buffer[i];
	if (parsing_status_ == INIT && read_size <= 0)	  // 클라이언트와 연결되어 있지만, 요청을 받은 상태는 아님
		return;
	if (parsing_status_ == INIT)
		parsing_status_ = START_LINE;
	if (read_size == 0 && parsing_status_ != DONE)	  // EOF
		checkEmptyRemainBuffer();
	if (read_size == -1)
		parsing_status_ = ERROR;
	if (parsing_status_ == ERROR || parsing_status_ == DONE)
		return;
	// remainbuf에 이어붙힌다.
	for (int i = 0; i < read_size; i++)
		buffer_.push_back(read_buffer[i]);
	if (buffer_.size() == 0) {	  ////////////////////
		parsing_status_ = ERROR;
		return;
	}

	v_c_iter line_start_it = buffer_.begin();

	while (line_start_it < buffer_.end()) {
		v_c_iter next_sep_it = getNextSepIter(line_start_it);
		std::vector<char> line(line_start_it, next_sep_it - 1);
		line_start_it = next_sep_it;
		if (next_sep_it != buffer_.end())
			line_start_it += 2;

		switch (parsing_status_) {
			case START_LINE:
				parseStartLine(line);
				break;
			case HEADER:
				parseHeader(line);
				break;
			case BODY:
				if (bodyType_ == DEFAULT) {
					if (next_sep_it != buffer_.end()) {
						line.push_back('\r');
						line.push_back('\n');
					}
					parseBody(line);
				}
				else if (bodyType_ == CHUNKED)
					func(line, line_start_it, next_sep_it);
				else if (bodyType_ == BINARY)
					parseBinaryBody(line);
				break;
			default:
				return;
		}
	}
	buffer_.clear();
}
// int main() {
//   Request req;
//   req.parse(0);

//   req.CheckEmptyRemainBuffer();
//   req.print();
// }


// TODO: header에서 나가는 경우 get, delete 일때 buffer 맞춰서 썰어주기
// multipartformdata일때 boundary-- 일때 끝나는 로직 만들어주기

