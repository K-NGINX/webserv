# compile options
NAME		= webserv
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
RM			= rm -f

# source files
SRCS		= modules/main.cpp											\
			  modules/Webserv.cpp										\
			  modules/Client/Request/Request.cpp						\
			  modules/Client/Response/Response.cpp						\
			  modules/Client/Client.cpp									\
			  modules/Client/ClientManager.cpp							\
			  modules/Client/RequestHandler.cpp							\
			  modules/Config/Block/ABlock.cpp							\
			  modules/Config/Block/Config.cpp							\
			  modules/Config/Block/LocationBlock.cpp					\
			  modules/Config/Block/ServerBlock.cpp						\
			  modules/Config/CommonDirectives/CommonDirectives.cpp		\
			  modules/Config/ConfigManager.cpp							\
			  modules/Server/Kqueue/Kqueue.cpp							\
			  modules/Server/ServerManager.cpp							\
			  modules/Utils/Utils.cpp
OBJS		= $(SRCS:.cpp=.o)

# colors
RESET		= \033[0m
GRAY		= \033[1;90m
RED			= \033[1;91m
GREEN		= \033[1;92m
YELLOW		= \033[1;93m
BLUE		= \033[1;94m
MAGENTA		= \033[1;95m
CYAN		= \033[1;96m

all: $(NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "$(GREEN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(GREEN)║                                               ║$(RESET)"
	@echo "$(GREEN)║                                               ║$(RESET)"
	@echo "$(GREEN)║                    webserv                    ║$(RESET)"
	@echo "$(GREEN)║                compile finished               ║$(RESET)"
	@echo "$(GREEN)║                                               ║$(RESET)"
	@echo "$(GREEN)║                                               ║$(RESET)"
	@echo "$(GREEN)╚═══════════════════════════════════════════════╝$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(CYAN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║              make clean finished              ║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════╝$(RESET)"

fclean:
	@make clean
	@$(RM) $(NAME)
	@echo "$(CYAN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║              make fclean finished             ║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════╝$(RESET)"

re:
	@make fclean
	@make all

build:
	@make all
	@make clean

.PHONY: all clean fclean re build

