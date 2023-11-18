#include "Kqueue.hpp"

Kqueue::Kqueue() : fd_(-1) {
	v_event_.resize(EVENT_SIZE);
}

Kqueue::~Kqueue() {}

int Kqueue::getEvents() {
	int event_cnt = kevent(fd_, v_change_.data(), v_change_.size(), v_event_.data(), EVENT_SIZE, NULL);
	// kevent()가 실패해서 -1인 경우 그냥 무시하고 계속 시도
	if (event_cnt != -1) v_change_.clear();
	return event_cnt;
}

void Kqueue::startMonitoringReadEvent(int ident, void *udata) { addChangeList(ident, EVFILT_READ, EV_ADD | EV_ENABLE, udata); }
void Kqueue::startMonitoringWriteEvent(int ident, void *udata) { addChangeList(ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, udata); }
void Kqueue::stopMonitoringReadEvent(int ident) { addChangeList(ident, EVFILT_READ, EV_DISABLE | EV_DELETE, NULL); }
void Kqueue::stopMonitoringWriteEvent(int ident) { addChangeList(ident, EVFILT_WRITE, EV_DISABLE | EV_DELETE, NULL); }

void Kqueue::addChangeList(int ident, int16_t filter, uint16_t flags, void *udata) {
	struct kevent new_event;
	EV_SET(&new_event, ident, filter, flags, 0, 0, udata);
	v_change_.push_back(new_event);
}
