#pragma once

#include "LocationBlock.hpp"

/**
 * @brief Location 블록을 하위 블록으로 가질 수 있음
 * 
 */
class ServerBlock : public ABlock {
public:
    ServerBlock();
    ServerBlock& operator=(const ServerBlock& other);

    /* getter */
    virtual ABlock* getLastSubBlock();
    const std::vector<LocationBlock>& getLocationBlockVec() const;
    const std::string& getIp() const;
    const std::string& getPort() const;
    const std::string& getServerName() const;
    const LocationBlock& findMatchingLocationBlock(std::string uri) const;

    virtual void addSubBlock(std::string& line);
    virtual void refineDirectives();

    void print() const;

private:
    void refineServerDirectives();
    void setHost(const std::string& value);

    size_t getMatchLength(const std::string& uri, const std::string& match_directive) const;

    std::vector<LocationBlock> v_location_block_;

    /* server 블록에서만 사용되는 지시어 */
    std::string ip_;
    std::string port_;
    std::string server_name_;
};