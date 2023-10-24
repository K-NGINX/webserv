#pragma once

enum ParsingRequestStatus {
	INIT,
	START_LINE,
	HEADER,
	EMPTY_LINE,
	BODY,
	DONE,
	ERROR // error면 파싱안하고
};

class Request {
public:
	Request();
	~Request();

private:
	ParsingRequestStatus status_;
	
};
