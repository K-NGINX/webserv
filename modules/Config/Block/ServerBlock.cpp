#include "ServerBlock.hpp"

ServerBlock::ServerBlock() :
ip_("0.0.0.0"),
port_("80"),
server_name_("") {}

ABlock* ServerBlock::getLastSubBlock() { return &(v_location_block_.back()); }

const std::vector<LocationBlock>& ServerBlock::getLocationBlockVec() const { return v_location_block_; }

const std::string& ServerBlock::getIp() const { return ip_; }

const std::string& ServerBlock::getPort() const { return port_; }

const std::string& ServerBlock::getServerName() const { return server_name_; }

size_t ServerBlock::getMatchLength(const std::string& uri, const std::string& match_directive) const {
    size_t length = 0;
    for (size_t i = 0; i < uri.size(); i++) {
        if (uri[i] == match_directive[i])
            length++;
    }
    return length;
}

/**
 * @brief 요청을 처리할 때 사용할 위치 블록을 리턴하는 함수
 * 
 * @note 일치하는 블록이 하나도 없다면 예외가 throw됨
 * 
 * @details [ 규칙 ]
 *      1. 위치 블록의 match_directive중 uri와 가장 길게 일치하는 위치 블록 리턴
 *      2. 길이가 같다면 먼저 나오는 위치 블록 리턴
 *      2. 만약 일치하는 블록이 하나도 없다면 404 에러
 */
const LocationBlock& ServerBlock::findMatchingLocationBlock(std::string uri) const {
    size_t size = v_location_block_.size(), idx = size, match_length = 0;
    for (size_t i = 0; i < size; i++) {
        const LocationBlock& location_block = v_location_block_[i];
        size_t length = getMatchLength(uri, location_block.getMatchDirective());
        if (length > match_length) {
            match_length = length;
            idx = i;
        }
    }
    if (idx == size)
        throw std::runtime_error("404"); ///////////////////////////
    else
        return v_location_block_[idx];
}

/**
 * @brief LocationBlock 객체를 벡터에 추가하는 함수
 *
 * @param line "location match_directive "일 것으로 기대됨
 */
void ServerBlock::addSubBlock(std::string& line) {
    // match_directive 추출
    Utils::trimWhiteSpace(line);
    size_t pos_sepatator = line.find_last_of(Utils::whitespace);
    std::string match_directive = line.substr(pos_sepatator + 1);

    // 블록 이름(location) 추출
    line = line.substr(0, pos_sepatator);
    Utils::trimWhiteSpace(line);

    if (match_directive == "" || line != "location")
        throw std::runtime_error("location blocks must start with the following format: \"location match_directive {\"");

    v_location_block_.push_back(LocationBlock(match_directive));
}

void ServerBlock::setHost(const std::string& value) {
    size_t pos_separator = value.find(':');
    if (pos_separator == std::string::npos) {
        if (value.find('.') != std::string::npos)
            ip_ = value;
        else
            port_ = value;
    } else {
        ip_ = value.substr(0, pos_separator);
        port_ = value.substr(pos_separator + 1);
    }
}

/**
 * @brief server 블록에서만 사용되는 지시어를 정제하는 함수
 * 
 */
void ServerBlock::refineServerDirectives() {
    std::map<std::string, std::string>::iterator directive_it = m_directives_.begin();
    while (directive_it != m_directives_.end()) {
        std::string directive = directive_it->first;
        std::string value = directive_it->second;

        if (directive == "listen")
            setHost(value);
        else if (directive == "server_name")
            server_name_ = value;

        directive_it++;
    }
}

void ServerBlock::refineDirectives() {
    refineServerDirectives();
    common_directives_.refine(m_directives_);
    print();

    std::vector<LocationBlock>::iterator location_it = v_location_block_.begin();
    while (location_it != v_location_block_.end()) {
        location_it->setCommonDirectives(common_directives_);
        location_it->refineDirectives();
        location_it++;
    }
}

void ServerBlock::print() const {
    std::cout << "[ SERVER ]" << std::endl;
    common_directives_.print();
    std::cout << "- ip: \"" << ip_ << "\"" << std::endl;
    std::cout << "- port: " << port_ << std::endl;
    std::cout << "- server_name: \"" << server_name_ << "\"" << std::endl;
}
