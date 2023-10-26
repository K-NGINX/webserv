#include <iostream>
#include <string>
#include <sstream>
#include "ClientManager.hpp"

// enum ParsingRequestStatus {
// 	START_LINE,
// 	HEADER,
// 	BODY,
// 	ERROR_PARSING // error면 파싱안하고
// };

std::vector<std::vector<char> > getLineVector(std::vector<char>& line) {
	std::vector<std::vector<char> > res;
	std::vector<char> temp;

	for (size_t i = 0; i < line.size(); ++i) {
		if (line[i] == '\r' && i < line.size() - 1 && line[i + 1] == '\n') {
			res.push_back(temp);
			temp.clear();
			i++;
		} else
			temp.push_back(line[i]);
	}
	if (!temp.empty()) {
		res.push_back(temp);
	}
	return res;
}

std::vector<char> getNewRemainBuffer(std::vector<char> &buffer) {
	std::vector<char> res;
	size_t last_idx = buffer.size() - 1;
	if (buffer[last_idx] != '\n' || buffer[last_idx - 1] != '\r') {
		size_t start_remain_unit_idx;
		for (int i = buffer.size() - 2; i >= 0; --i) {
			if (buffer[i] == '\r' && buffer[i+1] == '\n') {
				start_remain_unit_idx = i;
				break ;
			}
		}
		for (int i = start_remain_unit_idx + 2; i < buffer.size(); i++)
			res.push_back(buffer[i]);
		while (buffer.size() >= start_remain_unit_idx + 2)
			buffer.pop_back();
	}
	return res;
}

void parseStartLine(std::vector<char>& line, ParsingRequestStatus& status) {
	std::string s_line(line.begin(), line.end()), tmp;
	std::stringstream ss(s_line);

	std::vector<std::string> split;
	while (ss >> tmp)
		split.push_back(tmp);

	if (split.size() != 3
		|| (split[0] != "GET" && split[0] != "POST" && split[0] != "DELETE")
		|| split[2] != "HTTP/1.1") {
		std::cout << split[0] << std::endl;
		std::cout << split[1] << std::endl;
		std::cout << split[2] << std::endl;
		status = ERROR_PARSING;
		return ;
	}
	// method 등록
	// uri 형식인지 검사
	// uri 등록
	status = HEADER;
}

void parseHeader(std::vector<char>& line, ParsingRequestStatus& status) {
	if (line.size() == 0) {
		status = BODY;
		return ;
	}
	size_t sep_idx = std::find(line.begin(), line.end(), ':') - line.begin();
	if (sep_idx == line.size()) {
		status = ERROR_PARSING;
		return ;
	}
	// key value로 나눠서 넣어준다.
}

void parseBody(std::vector<char>& line, ParsingRequestStatus& status) {
	// 이건 멤버변수로 std::vector<std::vector<char> > v_body_ 만들고 거따가 다 추가
	// body가 꼭 있어야
}

int main() {
	char buffer[1024] = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nbody\r\nbod";
	std::vector<char> remain_buffer_;
	ParsingRequestStatus status_ = START_LINE;
	// remainbuf에 이어붙힌다.
	for (int i = 0; buffer[i]; i++)
		remain_buffer_.push_back(buffer[i]);

	std::vector<char> new_remain_buffer = getNewRemainBuffer(remain_buffer_);
	std::vector<std::vector<char> > v_splited_line = getLineVector(remain_buffer_);
	remain_buffer_ = new_remain_buffer;

	for (int i = 0; i < v_splited_line.size(); i++) {
		for (int j = 0; j < v_splited_line[i].size(); j++)
			std::cout << v_splited_line[i][j];
		std::cout << std::endl;
	}
	std::cout << "remain: ";
	for (int i = 0; i < new_remain_buffer.size(); i++)
		std::cout << new_remain_buffer[i];
	std::cout << std::endl;
	// 일단 테스트코드라 일회용 변수 사용
	for (int i = 0; i < v_splited_line.size(); i++) {
		switch (status_) {
			case START_LINE:
				parseStartLine(v_splited_line[i], status_);
				break ;
			case HEADER:
				parseHeader(v_splited_line[i], status_);
				break ;
			case BODY:
				parseBody(v_splited_line[i], status_);
				break ;
			case ERROR_PARSING:
				std::cerr << "error!!!" << std::endl;
				// socket 닫아주고 event막아줌;
				return 0; //////////
				break ;
		}
	}
}
