#pragma once

#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#define EVENT_SIZE 8

class Kqueue {
   public:
	Kqueue();
	~Kqueue();

	/* getter */
	int getEvents();

	void startMonitoringReadEvent(int ident, void *udata);
	void startMonitoringWriteEvent(int ident, void *udata);
	void stopMonitoringReadEvent(int ident);
	void stopMonitoringWriteEvent(int ident);

	int fd_;
	std::vector<struct kevent> v_change_;
	std::vector<struct kevent> v_event_;

   private:
	void addChangeList(int ident, int16_t filter, uint16_t flags, void *udata);
};
