#pragma once

#include <cstdlib>
#include <iostream>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>

class Kqueue {
public:
	Kqueue();
	~Kqueue();

	int getEvents();
	void registerReadEvent(int ident, void *udata);
	void registerWriteEvent(int ident, void *udata);
	void unregisterReadEvent(int ident);
	void unregisterWriteEvent(int ident);

	int fd_;
	std::vector<struct kevent> v_change_;
	std::vector<struct kevent> v_event_;

private:
	void addChangeList(int ident, int16_t filter, uint16_t flags, void *udata);
};
