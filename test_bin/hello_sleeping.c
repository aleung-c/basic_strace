#include <unistd.h>

int main(void)
{
	write(1, "Hello!\n", 8);
	sleep(20);
	return (0);
}
