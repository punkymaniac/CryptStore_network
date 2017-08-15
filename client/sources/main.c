#include <stdio.h>

#include "glob/core.h"
#include "client.h"

int			main(void)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run client\n");
	#endif

	int			sockd;

	if ((sockd = connect_daemon()) != -1)
	{
		run_shell();
	}

	#ifdef DEBUG
		dprintf(1, "=> Exit client\n");
	#endif
	return (0);
}
