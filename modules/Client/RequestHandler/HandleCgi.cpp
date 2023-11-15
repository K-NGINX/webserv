#include "RequestHandler.hpp"

static void execve_cgi(Client &client) {
    std::string root = ConfigManager::getInstance().getProgramPath() + client.location_->common_directives_.getRoot();
    // 실행 파일 경로 설정
    char *argv[3];
    argv[0] = const_cast<char*>("/usr/bin/python3");
    std::string filename = root + client.location_->getCgiPath();
    argv[1] = const_cast<char*>(filename.c_str());
    argv[2] = NULL;
    // 환경변수 설정
    char *envp[5];
    envp[0] = const_cast<char*>("REQUEST_METHOD=POST");
    envp[1] = const_cast<char*>("CONTENT_TYPE=multipart/form-data");
    std::string content_length = "CONTENT_LENGTH=" + client.request_.getContentLength();
    envp[2] = const_cast<char*>(content_length.c_str());
    std::string save_path = "SAVE_PATH=" + root + client.location_->getUploadPath();
    envp[3] = const_cast<char*>(save_path.c_str());
    envp[4] = NULL;
    // 실행
    execve(argv[0], argv, envp);
    std::cerr << RED << "execve fail" << RESET << std::endl;
    exit(1);
}

void RequestHandler::handleCgi(Client &client) {
	// 양방향 파이프 설정
	int p2c_fd[2];	  // 부모 -> 자식
	if (pipe(p2c_fd) == -1) {
		handleError(client, "500");
		return;
	}
	int c2p_fd[2];	  // 자식 -> 부모
	if (pipe(c2p_fd) == -1) {
		close(p2c_fd[0]);
		close(p2c_fd[1]);
		handleError(client, "500");
		return;
	}
	// 프로세스 복제
	client.pid_ = fork();
	if (client.pid_ < 0) {	  // fork 실패
		close(p2c_fd[0]);
		close(p2c_fd[1]);
		close(c2p_fd[0]);
		close(c2p_fd[1]);
		handleError(client, "500");
		return;
	} else if (client.pid_ == 0) {		   // 자식 프로세스
		dup2(p2c_fd[0], STDIN_FILENO);	   // 부모 -> 자식 읽기
		dup2(c2p_fd[1], STDOUT_FILENO);	   // 자식 -> 부모 쓰기
		close(p2c_fd[0]);
		close(p2c_fd[1]);
		close(c2p_fd[0]);
		close(c2p_fd[1]);
		// cgi 프로그램 실행
		execve_cgi(client);
	} else {	// 부모 프로세스
		close(p2c_fd[0]);
		close(c2p_fd[1]);
		// 양방향 파이프에 대한 읽기, 쓰기 이벤트 등록 및 클라이언트 상태 변경
		ServerManager::getInstance().kqueue_.startMonitoringReadEvent(c2p_fd[0], &client);
		ServerManager::getInstance().kqueue_.startMonitoringWriteEvent(p2c_fd[1], &client);
        client.status_ = WRITE_CGI;
	}
}
