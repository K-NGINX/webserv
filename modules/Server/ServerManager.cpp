#include "ServerManager.hpp"

ServerManager &ServerManager::getInstance()
{
	static ServerManager instance;
	return instance;
}

void ServerManager::closeAllServerSocket()
{
	for (int i = 0; i < v_server_socket_.size(); i++)
		close(v_server_socket_[i]);
}

void ServerManager::init()
{
	const std::vector<ServerBlock> &server_vec =
		ConfigManager::getInstance().getConfig().getServerBlockVec();

	for (size_t server_idx = 0; server_idx < server_vec.size(); server_idx++)
	{
		const char *hostname = server_vec[server_idx].getHost().c_str(); // config 불러서 넣어주기
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
		if (socket_socket == -1)
		{
			closeAllServerSocket();
			freeaddrinfo(host_info); // 메모리 해제
			throw std::runtime_error("Socket error");
		}
		v_server_socket_.push_back(socket_socket);

		if (bind(socket_socket, host_info->ai_addr, host_info->ai_addrlen) == -1)
		{
			closeAllServerSocket();
			freeaddrinfo(host_info); // 메모리 해제
			throw std::runtime_error("Bind error");
		}
		freeaddrinfo(host_info); // 메모리 해제

		// 연결 요청 대기 상태로 바꿔줌
		if (listen(socket_socket, 5) == -1)
		{
			closeAllServerSocket();
			throw std::runtime_error("Listen error");
		}
		fcntl(socket_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	}
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
void connectNewClient() {

}

void ServerManager::start()
{
	if (kqueue_.fd_ == -1)
	{
		closeAllServerSocket();
		throw std::runtime_error("Kqueue error");
	}

	for (size_t i = 0; i < v_server_socket_.size(); i++)
		kqueue_.makeNewEvent(v_server_socket_[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

	while (true)
	{
		int event_num = kevent(kqueue_.fd_, &(kqueue_.v_change_[0]), kqueue_.v_change_.size(), &(kqueue_.v_event_[0]), 8, NULL);

		if (event_num == -1) {
			// kevent 실패
			// 나중에 알아볼것
		} else if (event_num > 0) { // 이벤트 발생
			for (int i = 0; i < event_num; i++)
			{
				struct kevent new_event = kqueue_.v_event_[i];

				if (new_event.filter == EVFILT_READ) {
					if (isServerSocket(new_event.ident)) {
						connectNewClient();
					}std::cout << new_event.ident""ntManager::getInstance().isClientSocket(new_event.ident)) {
						// 클라이언트의 일을 함
					} else {
						// CGI, 파일 쓰기 등 다른 fd를 사용하는 작업
					}
				} else {

				}
			}
		}
	}
}
