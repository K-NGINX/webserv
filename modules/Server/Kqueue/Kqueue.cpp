#include <Kqueue.hpp>

Kqueue::Kqueue() : fd_(-1) {}

Kqueue::~Kqueue() {}

int Kqueue::getEvents() {
	int event_num = kevent(fd_, &v_change_[0], v_change_.size(), &v_event_[0], 8, NULL);
	if (event_num == -1)
		throw std::runtime_error("kevent error");
	v_change_.clear();
	return event_num;
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
