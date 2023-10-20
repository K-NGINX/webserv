#pragma once

#include <iostream>
#include <vector>

class CommonDirectives {
public:
    CommonDirectives();
    CommonDirectives& operator=(const CommonDirectives& other);

    /* setter */
    void setAutoindex(bool is_autoindex);
    void setClientMaxBodySize(int client_max_body_size);
    void setErrorCodeVec(const std::vector<std::string>& v_error_code);
    void setErrorPage(const std::string& error_page);
    void setIndexVec(const std::vector<std::string>& v_index);
    void setReturnCode(const std::string& return_code);
    void setReturnPath(const std::string& return_path);
    void setRoot(const std::string& root);

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