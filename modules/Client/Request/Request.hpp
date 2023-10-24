#pragma once

enum ParsingRequestStatus {
	INIT,
	START_LINE,
	HEADER,
	EMPTY_LINE,
	BODY,
	DONE
};

class Request {
public:
	Request();
	~Request();

private:
	ParsingRequestStatus status_;
};
