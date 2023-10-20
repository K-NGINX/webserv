#include "Webserv.hpp"

/**
 * @details [큰 흐름]
 *      1. conf 파일 파싱
 *      2. server 초기화
 *      3. kqueue 시작
 */
void Webserv::start(int argc, char** argv) {
    try {
        ConfigManager::getInstance().parse(argc, argv);
        // ServerManager::getInstance().initServer();
    } catch (std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
    }
}
