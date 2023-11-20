#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include "../../Config/ConfigManager.hpp"
#include "../../Server/ServerManager.hpp"
#include "../Client.hpp"

#define DEFAULT_ERROR_PAGE "/var/www/html/errors/default_error.html"

class RequestHandler {
   public:
	static void handleRequest(Client& client);
	static void handleError(Client& client, const std::string& error_code);
	static void handleCgi(Client& client);
	static void handleGet(Client& client);
	static void handleDelete(Client& client);

   private:
	RequestHandler();
	~RequestHandler();
};
