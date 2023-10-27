#pragma once

#include "ServerBlock.hpp"

/**
 * @brief Server 블록을 하위 블록으로 가질 수 있음
 * 
 */
class Config : public ABlock {
public:
    /* getter */
    virtual ABlock* getLastSubBlock();
    const std::vector<ServerBlock>& getServerBlockVec() const;
    const ServerBlock* findMatchingServerBlock(std::string host) const;

    virtual void addSubBlock(std::string& line);
    virtual void refineDirectives();
    virtual void print();

private:
    std::vector<ServerBlock> v_server_block_;
};