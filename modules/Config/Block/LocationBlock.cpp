#include "LocationBlock.hpp"

LocationBlock::LocationBlock(const std::string& match_directive) :
match_directive_(match_directive),
cgi_path_(""),
upload_path_("") {}

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
    const std::string whitespace = " \r\n\t\v\f";
    size_t pos_start = value.find_first_not_of(whitespace), pos_end;
    std::string method;

    while (pos_start != std::string::npos) {
        pos_end = value.find_first_of(whitespace, pos_start);

        method = value.substr(pos_start, pos_end - pos_start);
        if (method == "GET")
            v_allow_method_.push_back(GET);
        else if (method == "POST")
            v_allow_method_.push_back(POST);
        else if (method == "DELETE")
            v_allow_method_.push_back(DELETE);
        else
            throw ("invalid HTTP method");

        pos_start = value.find_first_not_of(whitespace, pos_end);
    }
}

void LocationBlock::refineDirectives() {
    /* location 블록에서만 사용되는 지시어 정제 */
    std::map<std::string, std::string>::iterator directive_it;
    if ((directive_it = m_directives_.find("allow_method")) != m_directives_.end())
        setAllowMethodVec(directive_it->second);
    if ((directive_it = m_directives_.find("cgi_path")) != m_directives_.end())
        cgi_path_ = directive_it->second;
    if ((directive_it = m_directives_.find("upload_path")) != m_directives_.end())
        upload_path_ = directive_it->second;

    print(); ///////////////////////////
}

void LocationBlock::print() {
    std::cout << "-- [LOCATION]" << std::endl;
    if (!v_allow_method_.empty()) {
        std::cout << "-- allow_method: ";
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
        std::cout << "-- cgi_path: " << cgi_path_ << std::endl;
    if (upload_path_ != "")
        std::cout << "-- upload_path: " << upload_path_ << std::endl;
}