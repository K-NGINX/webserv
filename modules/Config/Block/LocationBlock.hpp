#pragma once

#include "ABlock.hpp"

class LocationBlock : public ABlock {
public:
    LocationBlock(const std::string& match_directive);

    virtual void addSubBlock(std::string& line);
    virtual void refineDirectives();

    const std::string& getMatchDirective() const;
    const std::vector<std::string>& getAllowMethodVec() const;
    const std::string& getCgiPath() const;
    const std::string& getUploadPath() const;
    const std::string getPath() const;

    void print() const;
    bool isAllowMethod(const std::string& method) const;

private:
    void refineLocationDirectives();
    void setAllowMethodVec(std::string& value);

    std::string match_directive_;
    /* location 블록에서만 사용되는 지시어 */
    std::vector<std::string> v_allow_method_;
    std::string cgi_path_;
    std::string upload_path_;
};