#pragma once

enum ParsingRequestStatus {
	START_LINE,
	HEADER,
	EMPTY_LINE,
	BODY,
	DONE_PARSINGREQ,
	ERROR // error면 파싱안하고
};

class Request {
public:
	Request();
	~Request();

private:
	ParsingRequestStatus status_;
};
