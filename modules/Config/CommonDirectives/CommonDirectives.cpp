#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives() :
is_autoindex_(false),
client_max_body_size_(0),
error_page_(""),
return_code_(""),
return_path_(""),
root_("/"){}

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

void CommonDirectives::refine(std::map<std::string, std::string>& m_directives) {
    std::map<std::string, std::string>::iterator directive_it;

    if ((directive_it = m_directives.find("autoindex")) != m_directives.end())
        setAutoindex(directive_it->second);
    if ((directive_it = m_directives.find("client_max_body_size")) != m_directives.end())
        setClientMaxBodySize(directive_it->second);
    if ((directive_it = m_directives.find("error_page")) != m_directives.end())
        setErrorPage(directive_it->second);
    if ((directive_it = m_directives.find("index")) != m_directives.end())
        setIndex(directive_it->second);
    if ((directive_it = m_directives.find("return")) != m_directives.end())
        setReturn(directive_it->second);
    if ((directive_it = m_directives.find("root")) != m_directives.end())
        setRoot(directive_it->second);
}

const bool& CommonDirectives::isAutoindex() const { return is_autoindex_; }

const size_t& CommonDirectives::getClientMaxBodySize() const { return client_max_body_size_; }

const std::vector<std::string>& CommonDirectives::getErrorCodeVec() const { return v_error_code_; }

const std::string& CommonDirectives::getErrorPage() const { return error_page_; }

const std::vector<std::string>& CommonDirectives::getIndexVec() const { return v_index_; }

const std::string& CommonDirectives::getReturnCode() const { return return_code_; }

const std::string& CommonDirectives::getReturnPath() const { return return_path_; }

const std::string& CommonDirectives::getRoot() const { return root_; }

void CommonDirectives::setAutoindex(std::string& value) {
    Utils::trimWhiteSpace(value);
    if (value == "on")
        is_autoindex_ = true;
    else if (value != "off")
        throw std::runtime_error("autoindex directives must be \"on\" or \"off\"");
}

void CommonDirectives::setClientMaxBodySize(std::string& value) {
    Utils::trimWhiteSpace(value);
    std::istringstream iss(value);
    if (!(iss >> client_max_body_size_) || iss.eof() == false)
        throw std::runtime_error("client_max_body_size must be an integer");
}

void CommonDirectives::setErrorPage(std::string& value) {
    Utils::trimWhiteSpace(value);
    size_t pos_start = value.find_first_not_of(Utils::whitespace), pos_end;
    std::string error_str;

    while (pos_start != std::string::npos) {
        pos_end = value.find_first_of(Utils::whitespace, pos_start);

        error_str = value.substr(pos_start, pos_end - pos_start);
        if (error_str != "400" && error_str != "404" && error_str != "405" && error_str != "413" && error_str != "500")
            break;
        v_error_code_.push_back(error_str);

        pos_start = value.find_first_not_of(Utils::whitespace, pos_end);
    }
    if (error_str == "" || v_error_code_.empty())
        throw std::runtime_error("error_page must be in following format: \"error_code error_page\"");
    // 마지막 항목은 에러페이지
    error_page_ = error_str;

}

void CommonDirectives::setIndex(std::string& value) {
    Utils::trimWhiteSpace(value);
    size_t pos_start = value.find_first_not_of(Utils::whitespace), pos_end;
    std::string page;

    while (pos_start != std::string::npos) {
        pos_end = value.find_first_of(Utils::whitespace, pos_start);

        page = value.substr(pos_start, pos_end - pos_start);
        v_index_.push_back(page);

        pos_start = value.find_first_not_of(Utils::whitespace, pos_end);
    }
}

void CommonDirectives::setReturn(std::string& value) {
    Utils::trimWhiteSpace(value);
    size_t pos_sepatator = value.find_last_of(Utils::whitespace);
    return_path_ = value.substr(pos_sepatator + 1);

    return_code_ = value.substr(0, pos_sepatator);
    if (return_code_ != "301")
        throw std::runtime_error("return must be in following format: \"return_code return_path\"");
    Utils::trimWhiteSpace(return_code_);
}

void CommonDirectives::setRoot(std::string& value) {
    Utils::trimWhiteSpace(value);
    root_ = value;
}

void CommonDirectives::print() const {
    std::cout << "*- autoindex : " << (is_autoindex_ == true ? "on" : "off") << std::endl;
    std::cout << "*- client_max_body_size : " << client_max_body_size_ << std::endl;
    if (v_error_code_.empty() == false) {
        std::cout << "*- error_page : ";
        for (size_t i = 0; i < v_error_code_.size(); i++)
            std::cout << v_error_code_[i] << " ";
        std::cout << error_page_ << std::endl;
    }
    if (v_index_.empty() == false) {
        std::cout << "*- index : ";
        for (size_t i = 0; i < v_index_.size(); i++)
            std::cout << v_index_[i] << " ";
        std::cout << std::endl;
    }
    if (return_code_ != "")
        std::cout << "*- return : " << return_code_ << " " << return_path_ << std::endl;
    if (root_ != "")
        std::cout << "*- root : " << root_ << std::endl;
}
