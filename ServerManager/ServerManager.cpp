#include "ServerManager.hpp"

ServerManager& ServerManager::getInstance() {
	static ServerManager instance;
	return instance;
}

void ServerManager::initServer() {
	// confmanager 인스턴스 받아서
	int server_num; // TODO: == servervec.size()
	for (int server_idx = 0; server_idx < server_num; server_idx++) {
		struct sockaddr_in addr_struct;
		memset(&addr_struct, 0, sizeof(addr_struct));
		int socket_fd, port; // TODO: port 맞춰줘야함

		// 소켓 만들기
		if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw("Socket error");

		addr_struct.sin_family = AF_INET;
		addr_struct.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 ip접근허가
		addr_struct.sin_port = htons(port); // 엔디안 맞춰줌

		// 소켓에 현재 IP, port 등록
		if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&addr_struct), sizeof(addr_struct)) == -1)
			throw("Bind error");

		// 연결 요청 대기 상태로 바꿔줌
		if (listen(socket_fd, 5) == -1)
			throw("Listen error");
		fcntl(socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		v_server_socket_fd_.push_back(socket_fd);
		// kqueue 시작
	}
}
