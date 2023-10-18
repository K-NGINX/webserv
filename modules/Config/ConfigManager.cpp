#include "ConfigManager.hpp"

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

void ConfigManager::parse(int argc, char** argv) {

}

/**
 * @brief 재귀적으로 블록을 파싱하는 함수
 * 
 * @param ifs conf 파일 스트림
 * @param block 현재 파싱 중인 블록
 */
void parseBlock(std::ifstream& ifs, ConfigBlock* block) {

}