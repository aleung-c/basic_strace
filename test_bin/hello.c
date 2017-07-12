#include <unistd.h>

int main(void)
{
	write(1, "Hello!\n", 8);
	return (0);
}
