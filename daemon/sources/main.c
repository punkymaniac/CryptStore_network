#include <stdio.h>

#include "glob/core.h"
#include "network.h"

int			main(void)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run daemon\n");
	#endif

	run_server();

	#ifdef DEBUG
		dprintf(1, "=> Exit daemon\n");
	#endif
	return (0);
}
