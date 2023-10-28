#include "RequestHandler.hpp"

bool isFileType(std::string& file) {
	return access(file.c_str(), F_OK) == 0 && file.find('.') != std::string::npos;
}

bool isIndex(Client& client, std::string& file) {
	const std::vector<std::string>& v_index = client.location_->common_directives_.getIndexVec();
	for (size_t i = 0; i < v_index.size(); i++) {
		std::string temp = file + v_index[i];
		if (access(temp.c_str(), F_OK) == 0) {
			file = temp;
			return true;
		}
	}
	return false;
}

bool handleAutoindex(Client& client, std::string& file) {
	(void)client;
	(void)file;
	return true;
}

/**
 * @details [ 지시어 우선순위 ]
 *      1. root
 *      2. index
 *      3. autoindex
 */
void RequestHandler::handleGet(Client& client) {
	std::string root = ConfigManager::getInstance().getProgramPath() + client.location_->common_directives_.getRoot();
	std::string file = root + client.request_.uri_;
  
	if (isFileType(file) || isIndex(client, file) || handleAutoindex(client, file)) {
		int fd = open(file.c_str(), O_RDONLY);
		if (fd == -1)
			return handleError(client, "404");
		fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		ServerManager::getInstance().kqueue_.registerReadEvent(fd, &client);
		client.status_ = READ_FILE;
	}
}
