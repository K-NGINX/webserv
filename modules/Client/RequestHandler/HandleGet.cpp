#include "RequestHandler.hpp"

/**
 * @details [ 지시어 우선순위 ]
 *      1. root
 *      2. index
 *      3. autoindex
 */
void RequestHandler::handleGet(Client& client) {
	std::string root = ConfigManager::getInstance().getProgramPath() + client.location_->common_directives_.getRoot();
	std::string file = root + client.request_.uri_;

	//
	int fd = open(file.c_str(), O_RDONLY);
	if (fd == -1)
		return handleError(client, "404");
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	ServerManager::getInstance().kqueue_.registerReadEvent(fd, &client);
	client.status_ = READ_FILE;
}
