#include "Config.hpp"
#include "../ConfigManager.hpp"

ABlock* Config::getLastSubBlock() { return &(v_server_block_.back()); }

const std::vector<ServerBlock>& Config::getServerBlockVec() const { return v_server_block_; }

/**
 * @brief 요청을 처리할 때 사용할 서버 블록을 리턴하는 함수
 * 
 * @details [ 규칙 ]
 *      1. 먼저 ip와 port가 일치하는 서버 블록을 찾기
 *      2. 2개 이상이라면 server_name이 일치하는 서버 블록을 찾기
 *      3. 그래도 하나로 특정되지 않는다면 그 중 첫번 째 서버 블록을 리턴
 */
const ServerBlock& Config::findMatchingServerBlock(std::string host) const {
    // port 번호 구하기
    size_t pos_colon = host.find(':');
    std::string port = host.substr(pos_colon + 1);
    host = host.substr(pos_colon);
    // ip, server_name 구하기
    std::string ip = "0.0.0.0", server_name = "";
    if (host.find('.') != std::string::npos)
        ip = host;
    else
        server_name = host;
    // ip와 port를 기준으로 일치하는 서버 블록 찾기
    std::vector<int> v_match_idx;
    for (size_t i = 0; i < v_server_block_.size(); i++) {
        if (ip == v_server_block_[i].getIp() && port == v_server_block_[i].getPort())
            v_match_idx.push_back(i);
    }
    // 일치하는 것이 없다면 첫 번째 서버 블록 리턴
    if (v_match_idx.empty())
        return v_server_block_.front();
    // server_name을 기준으로 일치하는 서버 블록 찾기
    for (size_t i = 0; i < v_match_idx.size(); i++) {\
        if (server_name == v_server_block_[v_match_idx[i]].getServerName())
            return v_server_block_[v_match_idx[i]];
    }
    // 일치하는 것이 없다면 ip와 port가 일치했던 첫 번째 서버 블록 리턴
    return v_server_block_[v_match_idx.front()];
}

/**
 * @brief ServerBlock 객체를 벡터에 추가하는 함수
 *
 * @param line "server "일 것으로 기대됨
 */
void Config::addSubBlock(std::string& line) {
    Utils::trimWhiteSpace(line); // 서버 블록 이름(server) 추출
    if (line != "server")
        throw std::runtime_error("server blocks must start with the following format: \"server {\"");
    v_server_block_.push_back(ServerBlock());
}

void Config::refineDirectives() {
    std::vector<ServerBlock>::iterator it = v_server_block_.begin();
    while (it != v_server_block_.end()) {
        it->refineDirectives();
        it++;
    }
}
