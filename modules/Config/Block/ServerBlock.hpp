#pragma once

#include "ABlock.hpp"
#include "LocationBlock.hpp"

/**
 * @brief Location 블록을 하위 블록으로 가질 수 있음
 * 
 */
class ServerBlock : public ABlock {
public:
    virtual void addSubBlock(const std::string& line);
    virtual void refineDirectives();

private:
    std::vector<LocationBlock> v_location_block_;

    /* server 블록에서만 사용되는 지시어 */
    int port_;
    std::string server_name_;
};