#include <Kqueue.hpp>

Kqueue::Kqueue() : fd_(kqueue()) {}

Kqueue::~Kqueue() {}

void Kqueue::makeNewEvent(int ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
	struct kevent new_event;
	EV_SET(&new_event, ident, filter, flags, fflags, data, udata);
	v_change_.push_back(new_event);
}
