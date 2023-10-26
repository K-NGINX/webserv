#include "ServerManager.hpp"

ServerManager &ServerManager::getInstance() {
	static ServerManager instance;
	return instance;
}

void ServerManager::closeAllServerSocket() {
	for (int i = 0; i < v_server_socket_.size(); i++)
		close(v_server_socket_[i]);
}

void ServerManager::init() {
	// 서버 소켓 초기화
	const std::vector<ServerBlock> &server_vec = ConfigManager::getInstance().getConfig().getServerBlockVec();
	for (size_t server_idx = 0; server_idx < server_vec.size(); server_idx++) {
		const char *hostname = server_vec[server_idx].getIp().c_str(); // config 불러서 넣어주기
		const char *port = server_vec[server_idx].getPort().c_str();
		struct addrinfo protocol_info;
		struct addrinfo *host_info;

		// protocol_info 구성
		memset(&protocol_info, 0, sizeof(struct addrinfo));
		protocol_info.ai_family = AF_UNSPEC;	 // IPv4 또는 IPv6 모두 허용
		protocol_info.ai_socktype = SOCK_STREAM; // TCP 소켓

		int status = getaddrinfo(hostname, port, &protocol_info, &host_info);
		if (status != 0)
			throw std::runtime_error(gai_strerror(status));

		int socket_socket = socket(host_info->ai_family, host_info->ai_socktype, host_info->ai_protocol);
		if (socket_socket == -1) {
			closeAllServerSocket();
			freeaddrinfo(host_info); // 메모리 해제
			throw std::runtime_error("Socket error");
		}
		v_server_socket_.push_back(socket_socket);

		if (bind(socket_socket, host_info->ai_addr, host_info->ai_addrlen) == -1) {
			closeAllServerSocket();
			freeaddrinfo(host_info); // 메모리 해제
			throw std::runtime_error("Bind error");
		}
		freeaddrinfo(host_info); // 메모리 해제

		// 연결 요청 대기 상태로 바꿔줌
		if (listen(socket_socket, 5) == -1) {
			closeAllServerSocket();
			throw std::runtime_error("Listen error");
		}
		fcntl(socket_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	}
	// kqueue 초기화
	if ((kqueue_.fd_ = kqueue()) == -1) {
		closeAllServerSocket();
		throw std::runtime_error("Kqueue init error");
	}
	// 서버 소켓 read 이벤트 등록
	for (size_t i = 0; i < v_server_socket_.size(); i++)
		kqueue_.registerReadEvent(v_server_socket_[i], NULL);
}

bool ServerManager::isServerSocket(int fd) {
	return std::find(v_server_socket_.begin(), v_server_socket_.end(), fd) != v_server_socket_.end();
}

/**
 * @details [ 흐름 ]
 *		1. accept()로 클라이언트 소켓 만들기 실패 시 에러메시지만 띄우고 다음거 처리 ㄱ
 *		2. fcntl non-blockng 모드로 만들기
 *		3. 클라이언트 소켓에 대한 read, write 이벤트 kqueue에 등록
 *		4. ClientManager의 m_client_에 <client_socket, Client(client_socket)> 추가 !!!
 */
void ServerManager::connectNewClient(int server_fd) {
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_socket;
	if (client_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &client_len) == -1) {
		std::cerr << "Accept error" << std::endl;
		return ;
	}
	fcntl(client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Client new_client(client_socket); //////// 할당하지 않아도 괜찮은지 나중에 알아보기
	kqueue_.registerReadEvent(client_socket, &new_client);
	kqueue_.registerWriteEvent(client_socket, &new_client);
	ClientManager::getInstance().v_client_.push_back(new_client);
}

void ServerManager::handleEvent(struct kevent& event) {
	if (isServerSocket(event.ident)) // read 이벤트만 발생함
		connectNewClient(event.ident);
	else // 클라이언트 소켓, CGI fd, 파일 fd에서 read, write 이벤트가 발생할 수 있음
		ClientManager::getInstance().handleEvent(event);
}

void ServerManager::start() {
	while (true) {
		int event_cnt = kqueue_.getEvents();
		for (int i = 0; i < event_cnt; i++)
			handleEvent(kqueue_.v_event_[i]);
	}
}
