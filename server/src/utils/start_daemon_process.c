#include "server.h"
#include <signal.h>

void start_daemon_process() {
	pid_t pid = fork();
	
	if(pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid > 0) {
		exit(EXIT_SUCCESS);
	}

	if(setsid() < 0) {
		perror("setsid");
		exit(EXIT_FAILURE);
	}

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP,  SIG_IGN);

    pid = fork();

	if(pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid > 0) {
        printf("[SERVER] started PID=%d\n", pid);
		exit(EXIT_SUCCESS);
	}

	umask(0);

	int fd = open("/dev/null", O_RDWR);

    if (fd < 0) {
        exit(EXIT_FAILURE);
    }
	
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);

	signal(SIGTERM, SIG_DFL);
}

