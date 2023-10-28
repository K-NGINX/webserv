#include "RequestHandler.hpp"
#include "Client.hpp"

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

void RequestHandler::handleError(Client& client, const std::string& error_code) {
    // 에러 코드 설정
    client.response_.status_code_ = error_code;
    // 에러 페이지 설정
    std::string error_page = DEFAULT_ERROR_PAGE; // 기본 에러 페이지
    if (client.location_ != NULL) {
        const CommonDirectives& common_directives = client.location_->common_directives_;
        if (common_directives.isErrorCode(error_code)) // 사용자 정의 에러 페이지
            error_page = common_directives.getRoot() + common_directives.getErrorPage();
    } 
    int fd = open(error_page.c_str(), O_RDONLY);
    if (fd == -1)
        throw std::runtime_error("error page handle failed");
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    ServerManager::getInstance().kqueue_.registerReadEvent(fd, &client);
    client.status_ = READ_FILE;
}

// void RequestHandler::handleRedirection(Client& client) {
//     // status_code를 return_code로 설정
//     // 텍스트, 파일이라면 내용을 본문에 담고 헤더에 content-length 설정해서 보냄
//     // uri라면 location 헤더에 담아 보냄
//     // 헤더에 return code page 추가
//     // client.response.v_data_ 채워준 후 ?
//     client.status_ = SEND_RESPONSE;
// }

// 아직 모호함 ... :(
void RequestHandler::handleCgi(Client& client) {
//  - POST 요청이면 WRITE_CGI
//  - CGI 환경 변수 설정
//  - 양방향 파이프 resource_fd 생성
//  - fork()
//  - 자식 : CGI 프로그램 execve
//  - 부모 : waitpid
//  - GET 요청이면 READ_CGI
}

// autoindex 처리 아직 안함
void RequestHandler::handleGet(Client& client) {
    std::string uri = client.request_.uri_;
    std::string root = client.location_->common_directives_.getRoot();
    std::string file_name = (root == "/") ? uri : root + uri;
    if (file_name.back() != '/') file_name.push_back('/');
    if (uri.find('.') == std::string::npos) { // uri가 디렉토리인 경우 index 지시어에서 파일 찾기
        const std::vector<std::string>& v_index = client.location_->common_directives_.getIndexVec();
        for (size_t i = 0; i < v_index.size(); i++) {
            std::string temp = file_name + v_index[i];
            if (access(temp.c_str(), F_OK) == 0) { // 파일이 존재하면
                file_name = temp;
                break;
            }
        }
    }
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1)
        return handleError(client, "404");
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    ServerManager::getInstance().kqueue_.registerReadEvent(fd, &client);
    client.status_ = READ_FILE;
}

void RequestHandler::handlePost(Client& client) {
//  - 기존 파일 : 해당 파일 fd write 이벤트 등록 (201)
//  - 신규 파일 : 파일 fd write 이벤트 등록
    client.status_ = WRITE_FILE;
}

void RequestHandler::handleDelete(Client& client) {
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
void RequestHandler::handleRequest(Client& client) {
    Request& request = client.request_;
    if (request.parsing_status_ == ERROR)
        return handleError(client, "400"); // 잘못된 문법의 요청
    // 요청에 사용할 서버 블록과 위치 블록 찾기
    client.server_ = ConfigManager::getInstance().getConfig().findMatchingServerBlock(request.host_);
    client.location_ = client.server_->findMatchingLocationBlock(request.uri_);
    // 요청 파싱 결과 해석
    if (client.location_ == NULL)
        return handleError(client, "404"); // 요청은 적절하나 URI 없음
    else if (client.location_->isAllowMethod(request.method_) == false)
        return handleError(client, "405"); // 메소드 제한
    else if (client.location_->common_directives_.getClientMaxBodySize() < request.body_.size())
        return handleError(client, "413"); // 요청 객체 크기 초과
    else if (client.location_->getCgiPath() != "")
        return handleCgi(client);
    else if (request.method_ == "GET")
        return handleGet(client);
    else if (request.method_ == "POST")
        return handlePost(client);
    else if (request.method_ == "DELETE")
        return handleDelete(client);
}