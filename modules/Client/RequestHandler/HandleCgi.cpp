#include "RequestHandler.hpp"

static char *stringToCharPtr(std::string str) {
	char *result = new char[str.size() + 1];
	std::strcpy(result, str.c_str());
	return result;
}

static char **setArgv(Client &client) {
	std::string root = ConfigManager::getInstance().getProgramPath() + client.location_->common_directives_.getRoot();
	std::vector<std::string> v_argv;
	v_argv.push_back("/usr/bin/python3");
	v_argv.push_back(root + client.location_->getCgiPath());

	char **argv = new char *[v_argv.size() + 1];
	for (size_t i = 0; i < v_argv.size(); i++)
		argv[i] = stringToCharPtr(v_argv[i]);
	argv[v_argv.size()] = NULL;
	return argv;
}

static char **setEnv(Client &client) {
	std::string root = ConfigManager::getInstance().getProgramPath() + client.location_->common_directives_.getRoot();
	std::vector<std::string> v_env;
	v_env.push_back("REQUEST_METHOD=POST");
	v_env.push_back("CONTENT_TYPE=multipart/form-data; boundary=" + client.request_.getBoundary());
	v_env.push_back("CONTENT_LENGTH=" + client.request_.getContentLength());
	v_env.push_back("SAVE_PATH=" + root + client.location_->getUploadPath());
	std::string connection = client.is_keep_alive_ == true ? "keep-alive" : "Closed";
	v_env.push_back("CONNECTION=" + connection);

	char **env = new char *[v_env.size() + 1];
	for (size_t i = 0; i < v_env.size(); i++)
		env[i] = stringToCharPtr(v_env[i]);
	env[v_env.size()] = NULL;
	return env;
}

static void close_pipe_fd(std::vector<int> &pipe_fd) {
	for (size_t i = 0; i < pipe_fd.size(); i++)
		close(pipe_fd[i]);
}

void RequestHandler::handleCgi(Client &client) {
	std::vector<int> pipe_fd;
	// 양방향 파이프 설정
	int p2c_fd[2]; // 부모 -> 자식
	if (pipe(p2c_fd) == -1) {
		handleError(client, "500");
		return;
	}
	pipe_fd.push_back(p2c_fd[0]);
	pipe_fd.push_back(p2c_fd[1]);
	int c2p_fd[2]; // 자식 -> 부모
	if (pipe(c2p_fd) == -1) {
		close_pipe_fd(pipe_fd);
		handleError(client, "500");
		return;
	}
	pipe_fd.push_back(c2p_fd[0]);
	pipe_fd.push_back(c2p_fd[1]);
	// 프로세스 복제
	client.pid_ = fork();
	if (client.pid_ < 0) { // fork 실패
		close_pipe_fd(pipe_fd);
		handleError(client, "500");
		return;
	} else if (client.pid_ == 0) { // 자식 프로세스
		// close(c2p_fd[0]);
		// close(p2c_fd[1]);
		dup2(p2c_fd[0], STDIN_FILENO);	// 부모 -> 자식 읽기
		dup2(c2p_fd[1], STDOUT_FILENO); // 자식 -> 부모 쓰기
		close_pipe_fd(pipe_fd);
		// cgi 프로그램 실행
		char **argv = setArgv(client);
		char **envp = setEnv(client);
		chmod(argv[1], 0777);
		execve(argv[0], argv, envp);
		std::cerr << RED << "execve fail" << RESET << std::endl;
		delete[] argv;
		delete[] envp;
		exit(1);
	} else { // 부모 프로세스
		signal(SIGPIPE, SIG_IGN);
		close(p2c_fd[0]);
		close(c2p_fd[1]);
		// 양방향 파이프에 대한 읽기, 쓰기 이벤트 등록 및 클라이언트 상태 변경
		client.pipe_fd_[0] = c2p_fd[0];
		client.pipe_fd_[1] = p2c_fd[1];
		ServerManager::getInstance().kqueue_.startMonitoringReadEvent(client.pipe_fd_[0], &client);
		ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(client.pipe_fd_[1], &client);
		client.status_ = WRITE_CGI;
	}
}
