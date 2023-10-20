#include "ServerBlock.hpp"

ServerBlock::ServerBlock() :
host_("0.0.0.0"),
port_("80"),
server_name_("") {}

ABlock* ServerBlock::getLastSubBlock() { return &(v_location_block_.back()); }

const std::vector<LocationBlock>& ServerBlock::getLocationBlockVec() const { return v_location_block_; }

const std::string& ServerBlock::getHost() const { return host_; }

const std::string& ServerBlock::getPort() const { return port_; }

const std::string& ServerBlock::getServerName() const { return server_name_; }

/**
 * @brief LocationBlock 객체를 벡터에 추가하는 함수
 * 
 * @param line "location match_directive "일 것으로 기대됨
 */
void ServerBlock::addSubBlock(std::string& line) {
    // match_directive 추출
    Utils::refineStr(line);
    size_t pos_sepatator = line.find_last_of(Utils::whitespace);
    std::string match_directive = line.substr(pos_sepatator + 1);

    // 블록 이름(location) 추출
    line = line.substr(0, pos_sepatator);
    Utils::refineStr(line);

    if (match_directive == "" || line != "location")
        throw std::runtime_error("location blocks must start with the following format: \"location match_directive {\"");

    v_location_block_.push_back(LocationBlock(match_directive));
}

void ServerBlock::setHostPort(const std::string& value) {
    size_t pos_separator = value.find(':');
    if (pos_separator == std::string::npos) {
        if (value.find('.') != std::string::npos)
            host_ = value;
        else
            port_ = value;
    } else {
        host_ = value.substr(0, pos_separator);
        port_ = value.substr(pos_separator + 1);
    }
}

void ServerBlock::refineDirectives() {
    /* server 블록에서만 사용되는 지시어 정제 */
    std::map<std::string, std::string>::iterator directive_it;
    if ((directive_it = m_directives_.find("listen")) != m_directives_.end())
        setHostPort(directive_it->second);
    if ((directive_it = m_directives_.find("server_name")) != m_directives_.end())
        server_name_ = directive_it->second;
    print(); /////////////////
    common_directives_.refine(m_directives_);

    std::vector<LocationBlock>::iterator v_it = v_location_block_.begin();
    while (v_it != v_location_block_.end()) {
        v_it->setCommonDirectives(common_directives_);
        v_it->refineDirectives();
        v_it++;
    }
}

void ServerBlock::print() {
    std::cout << "[ SERVER ]" << std::endl;
    std::cout << "- host: \"" << host_ << "\"" << std::endl;
    std::cout << "- port: " << port_ << std::endl;
    std::cout << "- server_name: \"" << server_name_ << "\"" << std::endl;
}