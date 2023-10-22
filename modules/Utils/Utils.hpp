#pragma once

#include <iostream>

#include "HttpMethod.hpp"

class Utils
{
public:
    static void trimWhiteSpace(std::string &str);

    static std::string whitespace;

private:
    Utils();
    ~Utils();
};
