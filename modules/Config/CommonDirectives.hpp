#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class CommonDirectives {
public:
    CommonDirectives();
    CommonDirectives& operator=(const CommonDirectives& other);

    /* setter */
    void setAutoindex(const std::string& value);
    void setClientMaxBodySize(const std::string& value);
    void setErrorPage(const std::string& value);
    void setIndex(const std::string& value);
    void setReturn(const std::string& value);
    void setRoot(const std::string& value);

    void refineCommonDirectives(std::map<std::string, std::string>& m_directives);

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