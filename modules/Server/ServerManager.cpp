#include "ServerManager.hpp"

#include "../Client/ClientManager.hpp"

ServerManager::ServerManager() {}
ServerManager::~ServerManager() {}

ServerManager &ServerManager::getInstance() {
	static ServerManager instance;
	return instance;
}

void ServerManager::closeAllServerSocket() {
	for (size_t i = 0; i < v_server_socket_.size(); i++)
		close(v_server_socket_[i]);
}

void ServerManager::initServerSocket(const ServerBlock &server_block) {
	const char *hostname = server_block.getIp().c_str(); // config 불러서 넣어주기
	const char *port = server_block.getPort().c_str();
	struct addrinfo protocol_info;
	struct addrinfo *host_info;

	// protocol_info 구성
	memset(&protocol_info, 0, sizeof(struct addrinfo));
	protocol_info.ai_family = AF_UNSPEC;	 // IPv4 또는 IPv6 모두 허용
	protocol_info.ai_socktype = SOCK_STREAM; // TCP 소켓

	int status = getaddrinfo(hostname, port, &protocol_info, &host_info);
	if (status != 0)
		throw std::runtime_error(gai_strerror(status));

	// 소켓을 열어 v_server_socket에 등록해준다.
	int server_socket = socket(host_info->ai_family, host_info->ai_socktype, host_info->ai_protocol);
	if (server_socket == -1) {
		closeAllServerSocket();
		freeaddrinfo(host_info); // 메모리 해제
		throw std::runtime_error("Socket error");
	}
	v_server_socket_.push_back(server_socket);

	// 이전에 webserv를 시그널로 닫아 다시 bind할 때 이전 소켓 주소를 재사용 가능하게 해줌
	int enable = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
		closeAllServerSocket();
		freeaddrinfo(host_info); // 메모리 해제
		throw std::runtime_error("Setsockopt error");
	}

	// 주어진 소켓에 ip주소:port를 등록한다.
	if (bind(server_socket, host_info->ai_addr, host_info->ai_addrlen) == -1) {
		closeAllServerSocket();
		freeaddrinfo(host_info); // 메모리 해제
		throw std::runtime_error("Bind error");
	}
	freeaddrinfo(host_info); // 메모리 해제

	// 연결 요청 대기 상태로 바꿔줌
	if (listen(server_socket, 500) == -1) {
		closeAllServerSocket();
		throw std::runtime_error("Listen error");
	}

	// 논블락킹 모드로 변경
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
}

/**
 * @brief 클라이언트 연결 요청을 받는 서버 소켓을 만들고 kevent에 등록해주는 함수
 *
 */
void ServerManager::init() {
	// 서버 소켓 초기화
	const std::vector<ServerBlock> &server_vec = ConfigManager::getInstance().getConfig().getServerBlockVec();
	for (size_t i = 0; i < server_vec.size(); i++)
		initServerSocket(server_vec[i]);
	std::cout << GREEN << "📢 SERVER SOCKET INIT DONE" << RESET << std::endl;

	// kqueue 초기화
	if ((kqueue_.fd_ = kqueue()) == -1) {
		closeAllServerSocket();
		throw std::runtime_error("Kqueue init error");
	}
	std::cout << GREEN << "📢 KQUEUE INIT DONE" << RESET << std::endl;

	// 서버 소켓 read 이벤트 등록
	for (size_t i = 0; i < v_server_socket_.size(); i++)
		kqueue_.startMonitoringReadEvent(v_server_socket_[i], NULL);
}

bool ServerManager::isServerSocket(int fd) {
	return std::find(v_server_socket_.begin(), v_server_socket_.end(), fd) != v_server_socket_.end();
}

void ServerManager::connectNewClient(int server_socket) {
	int client_socket;
	if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
		std::cerr << RED << "Accept error" << RESET << std::endl;
		return;
	}
	fcntl(client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Client *new_client = new Client(client_socket);
	kqueue_.startMonitoringReadEvent(client_socket, new_client);
	ClientManager::getInstance().v_client_.push_back(new_client);
}

void ServerManager::handleEvent(struct kevent &event) {
	if (isServerSocket(event.ident))
		connectNewClient(event.ident);
	else // 클라이언트 소켓, CGI fd, 파일 fd에서 발생한 이벤트
		ClientManager::getInstance().handleEvent(event);
}

void ServerManager::start() {
	std::cout << GREEN << "📢 SERVER START" << RESET << std::endl;
	while (true) {
		int event_cnt = kqueue_.getEvents(); // 현재 쌓인 이벤트의 개수 리턴
		for (int i = 0; i < event_cnt; i++)
			handleEvent(kqueue_.v_event_[i]);
	}
}
