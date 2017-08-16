#include <stdio.h>

#include "glob/core.h"
#include "client.h"

int			main(void)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run client\n");
	#endif

	int							sockd;
	struct infod		*infod;

	infod = info_daemon();
	if ((sockd = connect_daemon()) != -1)
	{
		infod->socket = sockd;
		run_shell();
	}

	#ifdef DEBUG
		dprintf(1, "=> Exit client\n");
	#endif
	return (0);
}
