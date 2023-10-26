#include <iostream>
#include <string>
#include <sstream>
#include "ClientManager.hpp"


// bool	isValidStartLine(std::vector<char> line) {
// 	// get post delete

// }

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
		for (int i = buffer.size() - 2; i >= 0; --i)
        	if (buffer[i] == '\r' && buffer[i+1] == '\n')
				start_remain_unit_idx = i;
		for (int i = start_remain_unit_idx; i < buffer.size(); i++)
			res.push_back(buffer[i]);
		while (buffer.size() != start_remain_unit_idx + 1)
			buffer.pop_back();
	}
	return res;
}


int main() {
	char buffer[1024] = "startline\r\nheader\r\nheader\r\n\r\nbody\r\nbody";
	std::vector<char> remain_buffer_;
	ParsingRequestStatus status_ = START_LINE;
	// remainbuf에 이어붙힌다.
	for (int i = 0; buffer[i]; i++)
		remain_buffer_.push_back(buffer[i]);
	std::vector<char> new_remain_buffer = getNewRemainBuffer(remain_buffer_);
	std::vector<std::vector<char> > v_splited_line = getLineVector(remain_buffer_);
	for (int i = 0; i < v_splited_line.size(); i++) {
		for (int j = 0; j < v_splited_line[i].size(); j++)
			std::cout << v_splited_line[i][j];
		std::cout << std::endl;
	}
	// int line_idx = 0;
	// switch (status_) {
	// 	case START_LINE:
	// 		;
	// 		break ;
	// 	case HEADER:
	// 		;
	// 		break ;
	// 	case EMPTY_LINE:
	// 		;
	// 		break ;
	// 	case BODY:
	// 		;
	// 		break ;
	// 	case DONE:
	// 		;
	// 		break ;
	// 	case ERROR:
	// 		;
	// 		break ;

	// }

	// 첫 라인
}
