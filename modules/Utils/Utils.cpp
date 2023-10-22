#include "Utils.hpp"

std::string Utils::whitespace = " \r\n\t\v\f";

Utils::Utils() {}

Utils::~Utils() {}

void Utils::trimWhiteSpace(std::string& str) {
    str.erase(0, str.find_first_not_of(whitespace));
    str.erase(str.find_last_not_of(whitespace) + 1);
}
