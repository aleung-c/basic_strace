#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

int		main(void) {
	access("/bin/ls", F_OK);
	access("/usr/bin", F_OK);
	access("/home", F_OK);
	DIR *d = opendir("/");
	struct dirent *dd;
	while ((dd = readdir(d)))
		access(dd->d_name, F_OK | X_OK);
	open("/bin/ls", O_RDONLY);
	return (0);
}
