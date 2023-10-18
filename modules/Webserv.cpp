#include "Webserv.hpp"

void Webserv::start(int argc, char** argv) {
    try {
        ConfigManager::getInstance().parse(argc, argv);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}