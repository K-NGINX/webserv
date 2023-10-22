#pragma once

#include <cstdlib>
#include <iostream>
// #include <netdb.h>
// #include <netinet/in.h>
// #include <stdio.h>
// #include <string>
#include <sys/event.h>
// #include <sys/socket.h> // server.cpp
#include <sys/time.h>
#include <sys/types.h>
// #include <unistd.h>
#include <vector>

class Kqueue {
public:
	Kqueue();
	~Kqueue();

	void Kqueue::makeNewEvent(int ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);

	int fd_;
	std::vector<struct kevent> v_change_;
	std::vector<struct kevent> v_event_;
};
