#include "ServerBlock.hpp"

ServerBlock::ServerBlock() : ip_("0.0.0.0"), port_("80") {}

ABlock* ServerBlock::getLastSubBlock() { return &(v_location_block_.back()); }
const std::vector<LocationBlock>& ServerBlock::getLocationBlockVec() const { return v_location_block_; }
const std::string& ServerBlock::getIp() const { return ip_; }
const std::string& ServerBlock::getPort() const { return port_; }
const std::string& ServerBlock::getServerName() const { return server_name_; }

static size_t getMatchLength(const std::string& uri, const std::string& match_directive) {
	size_t length = 0;
	for (size_t i = 0; i < uri.size(); i++) {
		if (uri[i] == match_directive[i])
			length++;
		else
			break;
	}
	return length;
}

/**
 * @brief 요청을 처리할 때 사용할 위치 블록을 리턴하는 함수
 *
 * @note 일치하는 블록이 하나도 없다면 NULL이 리턴됨
 *
 * @details [ 규칙 ]
 *      1. 위치 블록의 match_directive중 uri와 가장 길게 일치하는 위치 블록 리턴
 *      2. 길이가 같다면 먼저 나오는 위치 블록 리턴
 *      2. 만약 일치하는 블록이 하나도 없다면 NULL 리턴
 */
const LocationBlock* ServerBlock::findMatchingLocationBlock(std::string uri) const {
	size_t size = v_location_block_.size(), idx = size, match_length = 0;
	for (size_t i = 0; i < size; i++) {
		const LocationBlock& location_block = v_location_block_[i];
		size_t length = getMatchLength(uri, location_block.getMatchDirective());
		if (length > match_length) {
			match_length = length;
			idx = i;
		}
	}
	if (idx == size) // 일치하는 블록이 없다
		return NULL;
	else
		return &(v_location_block_[idx]);
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
	std::map<std::string, std::string>::iterator directive_it;

	if ((directive_it = m_directives_.find("listen")) != m_directives_.end())
		setHost(directive_it->second);
	if ((directive_it = m_directives_.find("server_name")) != m_directives_.end())
		server_name_ = directive_it->second;
}

void ServerBlock::refineDirectives() {
	refineServerDirectives();
	common_directives_.refine(m_directives_);
	std::vector<LocationBlock>::iterator location_it = v_location_block_.begin();
	while (location_it != v_location_block_.end()) {
		location_it->common_directives_ = common_directives_; // 하위 블록인 위치 블록에 공통 지시어 공유
		location_it->refineDirectives();
		location_it++;
	}
}

void ServerBlock::print() {
	std::string indent = "    ";
	std::cout << indent << "[ SERVER ]" << std::endl;
	std::cout << indent << "- listen: " << ip_ << ":" << port_ << std::endl;
	std::cout << indent << "- server_name: " << server_name_ << std::endl;
	common_directives_.print(indent);

	std::vector<LocationBlock>::iterator location_it = v_location_block_.begin();
	while (location_it != v_location_block_.end()) {
		location_it->print();
		location_it++;
	}
}
