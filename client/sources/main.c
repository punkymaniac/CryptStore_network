#include <stdio.h>

#include "glob/core.h"

int			main(void)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run client\n");
	#endif

	#ifdef DEBUG
		dprintf(1, "=> Exit client\n");
	#endif
	return (0);
}
