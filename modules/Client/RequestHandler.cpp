#include "RequestHandler.hpp"
#include "Client.hpp"

void handleError(Client& client, const std::string& status_code) {
    client.response_.status_code_ = status_code;
    // client.response.v_data_ 채워준 후 ?
    client.status_ = SEND_RESPONSE;
}

void handleRedirection(Client& client) {
    // status_code를 return_code로 설정
    // 텍스트, 파일이라면 내용을 본문에 담고 헤더에 content-length 설정해서 보냄
    // uri라면 location 헤더에 담아 보냄
    // 헤더에 return code page 추가
    // client.response.v_data_ 채워준 후 ?
    client.status_ = SEND_RESPONSE;
}

void handleCgi(Client& client) {
//  - CGI 환경 변수 설정
//  - 양방향 파이프 resource_fd 생성
//  - fork()
//  - 자식 : CGI 프로그램 execve
//  - 부모 : resource fd read, write 이벤트 등록
    client.status_ = WRITE_CGI;
}

void handleGet(Client& client) {
//  - autoindex 지시어 on : 요청 경로 autoindex.html 생성 후 fd read 이벤트 등록
//  - 디렉토리 : index 찾아 해당 파일 fd read 이벤트 등록
//  - 파일명 : 해당 파일 fd read 이벤트 등록
//  - 없으면 404
    client.status_ = READ_FILE;
}

void handlePost(Client& client) {
//  - 기존 파일 : 해당 파일 fd write 이벤트 등록 (201)
//  - 신규 파일 : 파일 fd write 이벤트 등록
    client.status_ = WRITE_FILE;
}

void handleDelete(Client& client) {

}

/**
 * @details [ 요청 처리 순서 ]
 *      1. CGI (GET, POST)
 *      2. GET (정적)
 *      3. POST
 *      4. DELETE
 *      5. 리다이렉션
 */
void handleRequest(Client& client) {
    if (client.request_.parsing_status_ == ERROR)
        handleError(client, "400"); // 잘못된 요청
    // request 파싱 결과 해석
        // 서버 블록 찾기
        // 위치 블록 찾기 -> 404
        // 허용 메소드인지 검사 -> 405
        // 본문이 있다면 본문 크기 검사 -> 413
        // cgi_pass 지시어 : HandleCgi
        // GET : HandleGet
        // POST : HandlePost
        // DELETE : HandleDelete
        // return(리다이렉션) 지시어 : HandleRedirection
}