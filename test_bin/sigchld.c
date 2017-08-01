#include <stdio.h>
#include <signal.h>

int		main(void) {
	raise(SIGCHLD);
	printf("end ...\n");
	return (0);
}
