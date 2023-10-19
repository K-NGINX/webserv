#pragma once

#include <fstream>
#include <iostream>

#include "Block/ConfigBlock.hpp"

class ConfigManager {
public:
    static ConfigManager& getInstance();
    void parse(int argc, char** argv);
    void refineStr(std::string& str);

private:
    ConfigManager();
    ~ConfigManager();

    void checkLineType(std::string& line, char& line_type);
    void parseBlock(std::ifstream& ifs, ABlock* block);

    ConfigBlock config_;
    std::string conf_path_;
};