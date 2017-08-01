#include <unistd.h>

int		main(void) {
	for (int i = 0; i < 40000; i++)
		write(1, "\0", 1);
}
