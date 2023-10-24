#pragma once

#include "../../Utils/HttpMethod.hpp"
#include "ABlock.hpp"

class LocationBlock : public ABlock {
public:
    LocationBlock(const std::string& match_directive);
    LocationBlock& operator=(const LocationBlock& other);

    /* getter */
    const std::string& getMatchDirective() const;
    const std::vector<HttpMethod>& getAllowMethodVec() const;
    const std::string& getCgiPath() const;
    const std::string& getUploadPath() const;

    virtual void addSubBlock(std::string& line);
    virtual void refineDirectives();

    void print() const;

private:
    void refineLocationDirectives();
    void setAllowMethodVec(std::string& value);

    std::string match_directive_;

    /* location 블록에서만 사용되는 지시어 */
    std::vector<HttpMethod> v_allow_method_;
    std::string cgi_path_;
    std::string upload_path_;
};