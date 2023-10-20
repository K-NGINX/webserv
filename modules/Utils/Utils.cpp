#include "Utils.hpp"

Utils::Utils() {}

Utils::~Utils() {}

void Utils::refineStr(std::string& str) {
    const std::string whitespace = " \r\n\t\v\f";

    str.erase(0, str.find_first_not_of(whitespace));
    str.erase(str.find_last_not_of(whitespace) + 1);
}