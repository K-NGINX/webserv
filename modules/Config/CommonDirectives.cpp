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

void CommonDirectives::refineCommonDirectives(std::map<std::string, std::string>& m_directives) {
    std::map<std::string, std::string>::iterator directive_it;

    if ((directive_it = m_directives.find("autoindex")) != m_directives.end())
        setAutoindex(directive_it->second);
    if ((directive_it = m_directives.find("client_max_body_size")) != m_directives.end())
        setClientMaxBodySize(directive_it->second);
    if ((directive_it = m_directives.find("error_page")) != m_directives.end())
    if ((directive_it = m_directives.find("index")) != m_directives.end())
    if ((directive_it = m_directives.find("return")) != m_directives.end())
    if ((directive_it = m_directives.find("root")) != m_directives.end())
}

void CommonDirectives::setAutoindex(const std::string& value) {
    if (value == "on")
        is_autoindex_ = true;
    else if (value != "off")
        throw std::runtime_error("autoindex directives must be \"on\" or \"off\"");
}

void CommonDirectives::setClientMaxBodySize(const std::string& value) {
    std::istringstream iss(value);
    if (!(iss >> client_max_body_size_) || iss.eof() == false || client_max_body_size_ < 0)
        throw std::runtime_error("client_max_body_size must be an integer");
}

// void CommonDirectives::setErrorPage(const std::string& value) {

// }

// void CommonDirectives::setIndex(const std::string& value) {

// }

// void CommonDirectives::setReturn(const std::string& value) {

// }

// void CommonDirectives::setRoot(const std::string& value) {

// }