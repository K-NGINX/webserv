#include "RequestHandler.hpp"

void RequestHandler::handlePost(Client &client) {
	//  - 기존 파일 : 해당 파일 fd write 이벤트 등록 (201)
	//  - 신규 파일 : 파일 fd write 이벤트 등록
	client.status_ = WRITE_FILE;
}