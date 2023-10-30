#include "RequestHandler.hpp"

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

void RequestHandler::handleError(Client &client, const std::string &error_code) {
	client.response_.status_code_ = error_code;
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
	if (fd == -1)	 // 실패했다면 클라이언트 연결 끊어주기
		return client.setStatus(WILL_DISCONNECT);
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	ServerManager::getInstance().kqueue_.registerReadEvent(fd, &client);
	client.status_ = READ_FILE;
}

// 아직 모호함 ... :(
void RequestHandler::handleCgi(Client &client) {
	(void)client;
	//  - POST 요청이면 WRITE_CGI
	//  - CGI 환경 변수 설정
	//  - 양방향 파이프 resource_fd 생성
	//  - fork()
	//  - 자식 : CGI 프로그램 execve
	//  - 부모 : waitpid
	//  - GET 요청이면 READ_CGI
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
	request.print();
	if (request.parsing_status_ == ERROR)	 // 잘못된 문법의 요청
		return handleError(client, "400");
	if (request.connection_ == "close")
		client.is_keep_alive_ = false;
	// 요청에 사용할 서버 블록과 위치 블록 찾기
	client.server_ = ConfigManager::getInstance().getConfig().findMatchingServerBlock(request.host_);
	client.location_ = client.server_->findMatchingLocationBlock(request.uri_);
	// 요청 파싱 결과 해석
	if (client.location_ == NULL)
		return handleError(client, "404");	  // 요청은 적절하나 URI 없음
	else if (client.location_->isAllowMethod(request.method_) == false)
		return handleError(client, "405");	  // 메소드 제한
	else if (client.location_->common_directives_.getClientMaxBodySize() < request.body_.size())
		return handleError(client, "413");	  // 요청 객체 크기 초과
	else if (client.location_->getCgiPath() != "")
		return handleCgi(client);
	else if (request.method_ == "GET")
		return handleGet(client);
	else if (request.method_ == "DELETE")
		return handleDelete(client);
}
