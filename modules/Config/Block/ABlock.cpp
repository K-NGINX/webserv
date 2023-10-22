#include "ABlock.hpp"

ABlock::~ABlock() {}

ABlock* ABlock::getLastSubBlock() { return NULL; }

void ABlock::setCommonDirectives(CommonDirectives common_direcvites) { common_directives_ = common_direcvites; }

void ABlock::addDirective(const std::string& line) {
    size_t pos_separator = line.find(' ');
    if (pos_separator == std::string::npos)
        throw std::runtime_error("directives must be in following format: \"directive value\"");

    std::string key = line.substr(0, pos_separator);
    if (m_directives_.find(key) != m_directives_.end())
        throw std::runtime_error("directives can't be duplicated");

    std::string value = line.substr(pos_separator);
    Utils::trimWhiteSpace(value); // 앞뒤 공백 제거

    m_directives_[key] = value;
}
