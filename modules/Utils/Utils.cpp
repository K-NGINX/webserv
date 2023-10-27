#include "Utils.hpp"

std::string Utils::whitespace = " \r\n\t\v\f";

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

int Utils::stoi(std::string str) {
    return atoi(str.c_str());
}
