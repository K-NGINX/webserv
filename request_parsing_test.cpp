#include <iostream>
#include <string>
#include <sstream>
#include <vector>

bool isSep(char target, std::string& charset) {
	for (int i = 0; i < charset.size(); i++)
		if (charset[i] == target)
			return true;
	return false;
}

vector<string>	ft_split(string s, std::string charset) // 구분자 하나짜리
{
	vector<string>	res;
	int	start_idx = 0;

	for (int i = 0; i < s.size(); i++)
	{
		if (isSep(s[i], charset))
		{
			if (i > start_idx)
				res.push_back(s.substr(start_idx, i - start_idx));
			start_idx = i + 1;
		}
	}
		if (start_idx < s.size())
			res.push_back(s.substr(start_idx, s.size() - start_idx));
	return (res);
}


int main() {
	char buffer[1024] = "startline\r\nheader\nheader\n\nbody\nbod";
	std::string bod = "bod";
	std::string remain_buffer_;
	// int read_size = read(socket_, buffer,sizeof(buffer));
	// if (read_size == -1) {
		// 엌카쥐
	// }
	remain_buffer_ += std::string(buffer);
	std::stringstream ss(remain_buffer_);
	std::string line;
	while (getline(ss, line, '\r\n')) {
		std::cout << "line: " <<  line << std::endl;
	}
	remain_buffer_ = line;
	std::cout << remain_buffer_ << std::endl;
    return 0;
}
