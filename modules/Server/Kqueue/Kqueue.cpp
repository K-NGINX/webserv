#include "Kqueue.hpp"

Kqueue::Kqueue() : fd_(-1) {}

Kqueue::~Kqueue() {}

int Kqueue::getEvents() {
	int event_cnt = kevent(fd_, &v_change_[0], v_change_.size(), &v_event_[0], 8, NULL);
	// kevent()가 실패해서 -1인 경우 그냥 무시하고 계속 시도
	v_change_.clear();
	return event_cnt;
}

void Kqueue::registerReadEvent(int ident, void *udata) { addChangeList(ident, EVFILT_READ, EV_ADD | EV_ENABLE, udata); }

void Kqueue::registerWriteEvent(int ident, void *udata) { addChangeList(ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, udata); }

void Kqueue::unregisterReadEvent(int ident) { addChangeList(ident, EVFILT_READ, EV_DISABLE | EV_DELETE, NULL); }

void Kqueue::unregisterWriteEvent(int ident) { addChangeList(ident, EVFILT_WRITE, EV_DISABLE | EV_DELETE, NULL); }

void Kqueue::addChangeList(int ident, int16_t filter, uint16_t flags, void *udata) {
	struct kevent new_event;
	EV_SET(&new_event, ident, filter, flags, 0, 0, udata);
	v_change_.push_back(new_event);
}
