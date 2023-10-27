#pragma once

#include "Config/ConfigManager.hpp"
#include "Server/ServerManager.hpp"

# define RESET		"\033[0m"
# define GRAY		"\033[1;90m"
# define RED		"\033[1;91m"
# define GREEN		"\033[1;92m"
# define YELLOW		"\033[1;93m"
# define BLUE		"\033[1;94m"
# define MAGENTA	"\033[1;95m"
# define CYAN		"\033[1;96m"

class Webserv {
public:
    void start(int argc, char **argv);
};
