#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives() :
is_autoindex_(false),
client_max_body_size_(0),
error_page_(""),
return_code_(""),
return_path_(""),
root_(""){}

CommonDirectives& CommonDirectives::operator=(const CommonDirectives& other) {
    if (this != &other) {
        is_autoindex_ = other.is_autoindex_;
        client_max_body_size_ = other.client_max_body_size_;
        v_error_code_ = other.v_error_code_;
        error_page_ = other.error_page_;
        v_index_ = other.v_index_;
        return_code_ = other.return_code_;
        return_path_ = other.return_path_;
        root_ = other.root_;
    }
    return *this;
}

void CommonDirectives::setAutoindex(bool is_autoindex) {
    is_autoindex_ = is_autoindex;
}

void CommonDirectives::setClientMaxBodySize(int client_max_body_size) {
    client_max_body_size_ = client_max_body_size;
}

void CommonDirectives::setErrorCodeVec(const std::vector<std::string>& v_error_code) {
    v_error_code_ = v_error_code;
}

void CommonDirectives::setErrorPage(const std::string& error_page) {
    error_page_ = error_page;
}

void CommonDirectives::setIndexVec(const std::vector<std::string>& v_index) {
    v_index_ = v_index;
}

void CommonDirectives::setReturnCode(const std::string& return_code) {
    return_code_ = return_code;
}

void CommonDirectives::setReturnPath(const std::string& return_path) {
    return_path_ = return_path;
}

void CommonDirectives::setRoot(const std::string& root) {
    root_ = root;
}