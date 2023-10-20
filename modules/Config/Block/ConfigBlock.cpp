#include "ConfigBlock.hpp"
#include "../ConfigManager.hpp"

ABlock* ConfigBlock::getLastSubBlock() { return &(v_server_block_.back()); }

const std::vector<ServerBlock>& ConfigBlock::getServerBlockVec() const { return v_server_block_; }

/**
 * @brief ServerBlock 객체를 벡터에 추가하는 함수
 * 
 * @param line "server "일 것으로 기대됨
 */
void ConfigBlock::addSubBlock(std::string& line) {
    // 블록 이름(server) 추출
    Utils::refineStr(line);

    if (line != "server")
        throw std::runtime_error("server blocks must start with the following format: \"server {\"");

    v_server_block_.push_back(ServerBlock());
}

void ConfigBlock::refineDirectives() {
    std::vector<ServerBlock>::iterator it = v_server_block_.begin();
    while (it != v_server_block_.end()) {
        it->refineDirectives();
        it++;
    }
}