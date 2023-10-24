#include "LocationBlock.hpp"

LocationBlock::LocationBlock(const std::string& match_directive) :
match_directive_(match_directive),
cgi_path_(""),
upload_path_("") {}

LocationBlock& LocationBlock::operator=(const LocationBlock& other) {
    if (this != &other) {
        common_directives_ = other.common_directives_;
        m_directives_ = other.m_directives_;
        match_directive_ = other.match_directive_;
        v_allow_method_ = other.v_allow_method_;
        cgi_path_ = other.cgi_path_;
        upload_path_ = other.upload_path_;
    }
    return *this;
}

const std::vector<HttpMethod>& LocationBlock::getAllowMethodVec() const { return v_allow_method_;}

const std::string& LocationBlock::getCgiPath() const { return cgi_path_;}

const std::string& LocationBlock::getUploadPath() const { return upload_path_;}
/**
 * @note location 블록은 하위 블록을 가질 수 없으므로, 예외를 던짐
 * 
 */
void LocationBlock::addSubBlock(std::string& line) {
    (void) line;
    throw std::runtime_error("location blocks can't have sub-blocks");
}

void LocationBlock::setAllowMethodVec(std::string& value) {
    size_t pos_start = value.find_first_not_of(Utils::whitespace), pos_end;
    std::string method;

    while (pos_start != std::string::npos) {
        pos_end = value.find_first_of(Utils::whitespace, pos_start);

        method = value.substr(pos_start, pos_end - pos_start);
        if (method == "GET")
            v_allow_method_.push_back(GET);
        else if (method == "POST")
            v_allow_method_.push_back(POST);
        else if (method == "DELETE")
            v_allow_method_.push_back(DELETE);
        else
            throw std::runtime_error("invalid HTTP method");

        pos_start = value.find_first_not_of(Utils::whitespace, pos_end);
    }
}

/**
 * @brief location 블록에서만 사용되는 지시어를 정제하는 함수
 * 
 */
void LocationBlock::refineLocationDirectives() {
    std::map<std::string, std::string>::iterator directive_it = m_directives_.begin();
    while (directive_it != m_directives_.end()) {
        std::string directive = directive_it->first;
        std::string value = directive_it->second;

        if (directive == "allow_method")
            setAllowMethodVec(value);
        else if (directive == "cgi_path")
            cgi_path_ = value;
        else if (directive == "upload_path")
            upload_path_ = value;

        directive_it++;
    }
    print(); ///////////////////////////
}

void LocationBlock::refineDirectives() {
    refineLocationDirectives();
    common_directives_.refine(m_directives_);
}

void LocationBlock::print() {
    std::cout << "[LOCATION]" << std::endl;
    if (!v_allow_method_.empty()) {
        std::cout << "- allow_method: ";
        for (size_t i = 0; i < v_allow_method_.size(); i++) {
            if (v_allow_method_[i] == GET)
                std::cout << "GET ";
            else if (v_allow_method_[i] == POST)
                std::cout << "POST ";
            else if (v_allow_method_[i] == DELETE)
                std::cout << "DELETE ";
        }
        std::cout << std::endl;
    }
    if (cgi_path_ != "")
        std::cout << "- cgi_path: " << cgi_path_ << std::endl;
    if (upload_path_ != "")
        std::cout << "- upload_path: " << upload_path_ << std::endl;
}