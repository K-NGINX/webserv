#pragma once

#include <iostream>

#include "../Client.hpp"

#define DEFAULT_ERROR_PAGE "/var/www/html/default_error.html"

class RequestHandler {
   public:
	static void handleRequest(Client& client);
	static void handleError(Client& client, const std::string& error_code);
	// static void handleRedirection(Client& client);
	static void handleCgi(Client& client);
	static void handleGet(Client& client);
	static void handlePost(Client& client);
	static void handleDelete(Client& client);

   private:
	RequestHandler();
	~RequestHandler();
};
