#pragma once

#include <iostream>
#include <map>

class Response {
public:
    /* getter */
    const std::string& getStatusMessage() const;

private:
    std::string status_code_;
    std::map<std::string, std::string> m_header_;
    std::string body_content_;
};
