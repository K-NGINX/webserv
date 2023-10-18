#pragma once

#include "../../Utils/HttpMethod.hpp"
#include "ABlock.hpp"

class LocationBlock : public ABlock {
public:
    virtual void addSubBlock(const std::string& line);
    virtual void refineDirectives();

private:
    /* location 블록에서만 사용되는 지시어 */
    std::vector<HttpMethod> allow_method_;
    std::string cgi_path_;
    std::string upload_path_;
};