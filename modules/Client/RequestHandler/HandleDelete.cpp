#include "RequestHandler.hpp"

/**
 * @details [ 처리 방식 ]
 * 		1. uri에 해당하는 파일이 있으면 200 리턴
 * 		2. 없으면 204 리턴
 */
void RequestHandler::handleDelete(Client& client) {
	// 파일에 대한 절대 경로 구하기
	const CommonDirectives& common_directives = client.location_->common_directives_;
	std::string root = ConfigManager::getInstance().getProgramPath() + common_directives.getRoot();
	std::string resource = root + client.request_.getUri();
	// 파일이 있는지 확인 후 지우기
	if (access(resource.c_str(), F_OK) == 0) {
		if (std::remove(resource.c_str())) { // 파일 삭제를 실패한 경우
			handleError(client, "500");
			return;
		}
	} else { // 삭제할 파일이 없는 경우
		handleError(client, "404");
		return;
	}
	client.response_.setStatusCode("204");
	client.response_.makeResponse();
	ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(client.socket_, &client);
	client.status_ = SEND_RESPONSE;
}