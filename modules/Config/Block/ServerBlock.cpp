#include "ServerBlock.hpp"
#include "../ConfigManager.hpp"

ServerBlock::ServerBlock() :
host_("0.0.0.0"),
port_(80),
server_name_("") {}

/**
 * @brief LocationBlock 객체를 벡터에 추가하는 함수
 * 
 * @param line "location match_directive "일 것으로 기대됨
 */
void ServerBlock::addSubBlock(std::string& line) {
    // match_directive 추출
    ConfigManager::getInstance().refineStr(line);
    size_t pos_sepatator = line.find_last_of(' ');
    std::string match_directive = line.substr(pos_sepatator + 1);

    // 블록 이름(location) 추출
    line = line.substr(0, pos_sepatator);
    ConfigManager::getInstance().refineStr(line);

    if (match_directive == "" || line != "location")
        throw std::runtime_error("location blocks must start with the following format: \"location match_directive {\"");

    v_location_block_.push_back(LocationBlock(match_directive));
}

ABlock* ServerBlock::getLastSubBlock() {
    return &(v_location_block_.back());
}

void ServerBlock::setPort(const std::string& value) {
    int port;
    std::istringstream port_ss(value);
    if (!(port_ss >> port) || !port_ss.eof())
        throw std::runtime_error("port should be a value between 0 and 65535");
    
    port_ = port;
}

void ServerBlock::setHostPort(const std::string& value) {
    size_t pos_separator = value.find(':');
    if (pos_separator == std::string::npos) {
        if (value.find('.') != std::string::npos)
            host_ = value;
        else
            setPort(value);
    } else {
        host_ = value.substr(0, pos_separator);
        setPort(value.substr(pos_separator + 1));
    }
}

void ServerBlock::refineDirectives() {
    std::map<std::string, std::string>::iterator m_it = m_directives_.begin();
    while (m_it != m_directives_.end()) {
        std::string direcvite = m_it->first;
        std::string value = m_it->second;
        ConfigManager::getInstance().refineStr(value);

        if (direcvite == "listen")
            setHostPort(value);
        else if (direcvite == "server_name")
            server_name_ = value;

        m_it++;
    }
    print(); /////////////////

    std::vector<LocationBlock>::iterator v_it = v_location_block_.begin();
    while (v_it != v_location_block_.end()) {
        v_it->setCommonDirectives(common_directives_);
        v_it->refineDirectives();
        v_it++;
    }
}

void ServerBlock::print() {
    std::cout << "[ SERVER ]" << std::endl;
    std::cout << "host: \"" << host_ << "\"" << std::endl;
    std::cout << "port: " << port_ << std::endl;
    std::cout << "server_name: \"" << server_name_ << "\"" << std::endl;
}