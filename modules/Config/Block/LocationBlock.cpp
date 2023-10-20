#include "LocationBlock.hpp"
#include "../ConfigManager.hpp"

LocationBlock::LocationBlock(const std::string& match_directive) :
match_directive_(match_directive),
cgi_path_(""),
upload_path_("") {}

/**
 * @note location 블록은 하위 블록을 가질 수 없으므로, 예외를 던짐
 * 
 */
void LocationBlock::addSubBlock(std::string& line) {
    (void) line;
    throw std::runtime_error("location blocks can't have sub-blocks");
}

// " GET      POST"
void LocationBlock::setAllowMethodVec(std::string& value) {
    const std::string whitespace = " \r\n\t\v\f";
    size_t pos_start, pos_end;
    std::string method;

    while (true) {
        pos_start = value.find_first_not_of(whitespace);
        pos_end = value.substr(pos_start).find_first_of(whitespace);

        method = value.substr(pos_start, pos_end - pos_start + 1);
        value = value.substr(pos_end + 1);

        if (method == "GET")
            v_allow_method_.push_back(GET);
        else if (method == "POST")
            v_allow_method_.push_back(POST);
        else if (method == "DELETE")
            v_allow_method_.push_back(DELETE);
        else
            throw ("invalid HTTP method");

        if (pos_end == std::string::npos)
            break;
    }
}

void LocationBlock::refineDirectives() {
    std::map<std::string, std::string>::iterator m_it = m_directives_.begin();
    while (m_it != m_directives_.end()) {
        std::string direcvite = m_it->first;
        std::string value = m_it->second;

        if (direcvite == "allow_method")
            setAllowMethodVec(value);
        else if (direcvite == "cgi_path")
            cgi_path_ = value;
        else if (direcvite == "upload_path")
            upload_path_ = value;

        m_it++;
    }

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