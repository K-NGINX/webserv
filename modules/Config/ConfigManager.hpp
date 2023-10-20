#pragma once

#include <fstream>
#include <iostream>

#include "../Utils/Utils.hpp"
#include "Block/ConfigBlock.hpp"

class ConfigManager {
public:
    static ConfigManager& getInstance();
    void parse(int argc, char** argv);

private:
    ConfigManager();
    ~ConfigManager();

    void checkLineType(std::string& line, char& line_type);
    void parseBlock(std::ifstream& ifs, ABlock* block);

    ConfigBlock config_;
    std::string conf_path_;
};