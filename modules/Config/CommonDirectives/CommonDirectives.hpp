#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "../../Utils/Utils.hpp"

class CommonDirectives {
public:
    CommonDirectives();
    CommonDirectives& operator=(const CommonDirectives& other);

    /* setter */
    void setAutoindex(std::string& value);
    void setClientMaxBodySize(std::string& value);
    void setErrorPage(std::string& value);
    void setIndex(std::string& value);
    void setReturn(std::string& value);
    void setRoot(std::string& value);

    void refine(std::map<std::string, std::string>& m_directives);
    void print() const;

private:
    bool is_autoindex_;
    int client_max_body_size_;
    std::vector<std::string> v_error_code_;
    std::string error_page_;
    std::vector<std::string> v_index_;
    std::string return_code_;
    std::string return_path_;
    std::string root_;
};