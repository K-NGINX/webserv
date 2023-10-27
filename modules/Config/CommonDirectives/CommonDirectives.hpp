#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "../../Utils/Utils.hpp"

class CommonDirectives {
public:
    CommonDirectives();
    CommonDirectives& operator=(const CommonDirectives& other);

    /* getter */
    const bool& isAutoindex() const;
    const size_t& getClientMaxBodySize() const;
    const std::vector<std::string>& getErrorCodeVec() const;
    const std::string& getErrorPage() const;
    const std::vector<std::string>& getIndexVec() const;
    const std::string& getReturnCode() const;
    const std::string& getReturnPath() const;
    const std::string& getRoot() const;
    /* setter */
    void setAutoindex(std::string& value);
    void setClientMaxBodySize(std::string& value);
    void setErrorPage(std::string& value);
    void setIndex(std::string& value);
    void setReturn(std::string& value);
    void setRoot(std::string& value);

    void refine(std::map<std::string, std::string>& m_directives);
    void print(const std::string& indent) const;

private:
    bool is_autoindex_;
    size_t client_max_body_size_;
    std::vector<std::string> v_error_code_;
    std::string error_page_;
    std::vector<std::string> v_index_;
    std::string return_code_;
    std::string return_path_;
    std::string root_;
};