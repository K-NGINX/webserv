#include "ConfigManager.hpp"

ConfigManager::ConfigManager() : conf_path_("default.conf") {}

ConfigManager::~ConfigManager() {}

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

const Config& ConfigManager::getConfig() const { return config_; }

void ConfigManager::parse(int argc, char** argv) {
    /* 프로그램 인자 검사 */
    if (argc > 2)
        throw std::runtime_error("usage: ./webserv [configuration file]");
    else if (argc == 2)
        conf_path_ = argv[1];

    /* conf 파일 오픈 */
    std::ifstream ifs(conf_path_.c_str());
    if (ifs.fail())
        throw std::runtime_error("configuration file can't open");

    /* 파싱 */
    parseBlock(ifs, &config_);
    config_.refineDirectives();
}

void ConfigManager::checkLineType(std::string& line, char& line_type) {
    bool is_block_end = line.back() == '}' ? true : false;
    bool is_directives = line.back() == ';' ? true : false;
    bool is_block_start = line.back() == '{' ? true : false;
    line.pop_back(); // 타입 확인 후 맨 뒤 문자 제거

    if (is_block_end && !is_directives && !is_block_start)
        line_type = '}';
    else if (!is_block_end && is_directives && !is_block_start)
        line_type = ';';
    else if (!is_block_end && !is_directives && is_block_start)
        line_type = '{';
    else
        line_type = '\0';
}

/**
 * @brief 재귀적으로 블록을 파싱하는 함수
 *
 * @param ifs conf 파일 스트림
 * @param block 현재 파싱 중인 블록
 */
void ConfigManager::parseBlock(std::ifstream& ifs, ABlock* block) {
    std::string line;
    char line_type;

    while (std::getline(ifs, line)) {
        Utils::trimWhiteSpace(line);
        if (line == "")
            continue;
        checkLineType(line, line_type);
        switch (line_type) {
        case '}':
            return;
        case ';':
            block->addDirective(line);
            break;
        case '{':
            block->addSubBlock(line);
            parseBlock(ifs, block->getLastSubBlock());
            break;
        default:
            throw std::runtime_error("configuration file is wrong ...\nline : \"" + line + "\"");
        }
    }
}
