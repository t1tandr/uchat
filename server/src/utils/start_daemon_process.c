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
	if(chdir("/") < 0) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}

	// struct rlimit lim;
	// getrlimit(RLIMIT_NOFILE, &lim);

	for (int fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}
}

