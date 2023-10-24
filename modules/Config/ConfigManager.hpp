#pragma once

#include <fstream>
#include <iostream>

#include "../Utils/Utils.hpp"
#include "Block/Config.hpp"

class ConfigManager {
public:
    static ConfigManager& getInstance();
    const Config& getConfig() const;

    void parse(int argc, char** argv);

private:
    ConfigManager();
    ~ConfigManager();

    void checkLineType(std::string& line, char& line_type);
    void parseBlock(std::ifstream& ifs, ABlock* block);

    Config config_;
    std::string conf_path_;
};
