#pragma once

#include <fstream>
#include <iostream>

#include "ConfigBlock.hpp"

class ConfigManager {
public:
    static ConfigManager& getInstance();
    void parse(int argc, char** argv);

private:
    ConfigManager();
    ~ConfigManager();

    void parseBlock(std::ifstream& ifs, ConfigBlock* block);

    ConfigBlock config_;
    std::string conf_path_;
};