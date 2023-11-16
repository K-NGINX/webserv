#include "RequestHandler.hpp"

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

void RequestHandler::handleError(Client &client, const std::string &error_code) {
	client.response_.setStatusCode(error_code);
	// 에러 페이지 설정
	std::string error_page = DEFAULT_ERROR_PAGE;	// 기본 에러 페이지
	if (client.location_ != NULL) {
		const CommonDirectives &common_directives = client.location_->common_directives_;
		if (common_directives.isErrorCode(error_code))
			error_page = common_directives.getRoot() + common_directives.getErrorPage();	// 사용자 정의 에러 페이지
	}
	error_page = ConfigManager::getInstance().getProgramPath() + error_page;
	// 에러 페이지에 대한 fd 읽기 이벤트 등록
	int fd = open(error_page.c_str(), O_RDONLY);
	if (fd == -1) {	 // 실패했다면 클라이언트 연결 끊어주기
		client.setStatus(WILL_DISCONNECT);
		return ;
	}
	// 파일 형식에 따른 Content-Type 설정 후 응답 보내기
	client.response_.setContentType(error_page);
	std::cout << YELLOW << "error page : " << error_page << RESET << std::endl; ////////////////////
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC); /////////////////////
	ServerManager::getInstance().kqueue_.startMonitoringReadEvent(fd, &client);
	client.status_ = READ_FILE;
}

void RequestHandler::handleDelete(Client &client) {
	//  - 삭제 파일 있음 : 200
	//  - 삭제 파일 없음 : (204)
	(void)client;
}

/**
 * @details [ 요청 처리 순서 ]
 *      1. CGI (GET, POST)
 *      2. GET (정적)
 *      3. POST
 *      4. DELETE
 *      5. 리다이렉션
 */
void RequestHandler::handleRequest(Client &client) {
	Request &request = client.request_;
	request.print();						 ////////////////////////
	if (request.getParsing_status() == ERROR) {	 // 잘못된 문법의 요청
		handleError(client, "400");
		return ;
	}
	if (request.getConnection() == "Closed")
		client.is_keep_alive_ = false;
	// 요청에 사용할 서버 블록과 위치 블록 찾기
	client.server_ = ConfigManager::getInstance().getConfig().findMatchingServerBlock(request.getHost());
	client.location_ = client.server_->findMatchingLocationBlock(request.getUri());
	// 요청 파싱 결과 해석
	if (client.location_ == NULL)
		handleError(client, "404");	  // 요청은 적절하나 URI 없음
	else if (client.location_->isAllowMethod(request.getMethod()) == false)
		handleError(client, "405");	  // 메소드 제한
	else if (client.location_->checkBodySize(request.getBodySize()) == false)
		handleError(client, "413");	  // 요청 객체 크기 초과
	else if (client.location_->getCgiPath() != "")
		handleCgi(client);
	else if (request.getMethod() == "GET")
		handleGet(client);
	else if (request.getMethod() == "DELETE")
		handleDelete(client);
}
