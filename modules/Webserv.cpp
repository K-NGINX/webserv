#include "Webserv.hpp"

void Webserv::start(int argc, char** argv) {
    try {
        ConfigManager::getInstance().parse(argc, argv);
    } catch (std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
    }
}