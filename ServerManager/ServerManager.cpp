#include "ServerManager.hpp"

ServerManager& ServerManager::getInstance() {
	static ServerManager instance;
	return instance;
}


void ServerManager::initServer() {
	// confmanager 인스턴스 받아서
	int server_num; // TODO: == servervec.size()
	for (int server_idx = 0; server_idx < server_num; server_idx++) {

		const char* hostname = "30.0.0.0"; // config 불러서 넣어주기
		const char* port = "1";
		struct addrinfo protocol_info;
		struct addrinfo *host_info;

		// protocol_info 구성
		memset(&protocol_info, 0, sizeof(struct addrinfo));
		protocol_info.ai_family = AF_UNSPEC;     // IPv4 또는 IPv6 모두 허용
		protocol_info.ai_socktype = SOCK_STREAM; // TCP 소켓

		int status = getaddrinfo(hostname, port, &protocol_info, &host_info);

		if (status != 0)
			throw std::runtime_error(gai_strerror(status));

		int socket_fd = socket(host_info->ai_family, host_info->ai_socktype, host_info->ai_protocol);
		if (socket_fd == -1) {
			freeaddrinfo(host_info); // 메모리 해제
			throw std::runtime_error("Socket error");
		}

		if (bind(socket_fd, host_info->ai_addr, host_info->ai_addrlen) == -1) {
			close(socket_fd);
			freeaddrinfo(host_info); // 메모리 해제
			throw std::runtime_error("Bind error");
		}
		freeaddrinfo(host_info); // 메모리 해제

		// 연결 요청 대기 상태로 바꿔줌
		if (listen(socket_fd, 5) == -1) {
			close(socket_fd);
			throw std::runtime_error("Listen error");
		}
		fcntl(socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		v_server_socket_fd_.push_back(socket_fd);
		// kqueue 시작
	}
}
