#include <unistd.h>

int			main(void)
{
	write(1, "Hello main test\n", 16);
	return (0);
}
