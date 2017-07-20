#include <stdio.h>

#include "global.h"
#include "network.h"

int			main(void)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run main test\n");
	#endif

	run_server();

	#ifdef DEBUG
		dprintf(1, "=> Exit main test\n");
	#endif
	return (0);
}
