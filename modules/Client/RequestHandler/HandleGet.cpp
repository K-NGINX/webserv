#include "RequestHandler.hpp"

static bool isFileType(std::string& resource) {
	// 점('.')이 포함되어 있고, 해당 경로에 존재하면 파일 타입이라고 판단
	return resource.find('.') != std::string::npos && access(resource.c_str(), F_OK) == 0;
}

static bool isIndex(Client& client, std::string& resource) {
	const std::vector<std::string>& v_index = client.location_->common_directives_.getIndexVec();
	// index 배열을 순회하면서 존재하는 파일이 있으면 true 리턴
	for (size_t i = 0; i < v_index.size(); i++) {
		std::string temp = resource + "/" + v_index[i];
		if (access(temp.c_str(), F_OK) == 0) {
			resource = temp;
			return true;
		}
	}
	return false;
}

static void handleAutoindex(Client& client, const std::string& resource, std::string uri) {
	// html 코드 생성
	std::string autoindex_html = "<html><body><h1>" + uri.substr(1) + " List</h1><ul>";
	autoindex_html += "<table>";
	// 목록 채우기
	struct dirent* entry;
	DIR* dp = opendir(resource.c_str());
	if (dp != NULL) {
		while ((entry = readdir(dp))) {
			std::string entry_name = entry->d_name;
			if (uri.back() != '/') uri.push_back('/');
			std::string entry_path = uri + entry_name;
			if (entry_name == "." || entry_name == "..") continue;
			// 각 파일에 대한 링크는 요청한 URL에 대한 상대 경로
			autoindex_html += "<tr><td><a href='" + entry_path + "'>" + entry_name + "</a></td></tr>";
		}
		closedir(dp);
	}
	autoindex_html += "</table></ul></body></html>";
	// 응답 body에 써주기
	client.response_.setBody(std::vector<char>(autoindex_html.begin(), autoindex_html.end()));
	client.response_.makeResponse();
	ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(client.socket_, &client);
	client.status_ = SEND_RESPONSE;
}

/**
 * @details [ uri에 따른 처리 방식 ]
 * 		1. uri가 파일 형식
 * 		- uri에 해당하는 파일이 있으면 리턴, 없으면 404 리턴
 * 		2. uri가 디렉토리 형식
 * 		- 기본 파일(index)이 있는지 찾음
 * 		- 기본 파일이 없고 autoindex 지시어가 "on"이면 목록 페이지 전송
 * 		- 아무것도 해당되지 않으면 404 에러
 */
void RequestHandler::handleGet(Client& client) {
	// 파일에 대한 절대 경로 구하기
	const CommonDirectives& common_direcvties = client.location_->common_directives_;
	std::string root = ConfigManager::getInstance().getProgramPath() + common_direcvties.getRoot();
	std::string resource = root + client.request_.getUri();
	if (resource.back() == '/')
		resource.pop_back();
	// uri가 디렉토리 형식이고 기본 파일이 없는데 autoindex가 "on"이면 -> autoindex 처리
	if (isFileType(resource) == false && isIndex(client, resource) == false && common_direcvties.isAutoindex()) {
		handleAutoindex(client, resource, client.request_.getUri());
		return ;
	}
	// uri가 파일 형식이라면 uri 자체를, 디렉토리 형식이라면 기본 파일(index)을 사용
	client.file_fd_ = open(resource.c_str(), O_RDONLY);
	if (client.file_fd_ == -1) {
		handleError(client, "404");
		return ;
	}
	// 파일 형식에 따른 Content-Type 설정 후 응답 보내기
	client.response_.setContentType(resource);
	ServerManager::getInstance().kqueue_.startMonitoringReadEvent(client.file_fd_, &client);
	client.status_ = READ_FILE;
}
