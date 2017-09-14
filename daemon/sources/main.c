#include <stdio.h>
#include <string.h>

#include "glob/core.h"
#include "server.h"
#include "argument.h"

static void								arg_port(int nbrarg, char **av)
{
	(void)nbrarg;
	(void)av;
}

static struct argument		*st_get_arg(void)
{
	static struct argument	array[10] = {
		{"-p", "--port", 1, arg_port},
		{0x00, 0x00, 0, 0x00}
	};
	return (array);
}

int			main(int ac, char **av)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run daemon\n");
	#endif

	struct argument		*arg;
	int								n;
	int								i;

	if (ac > 1)
	{
		arg = st_get_arg();
		for (n = 0; n < ac; ++n)
		{
			if (av[n][0] == SEPARATOR_OPTION)
			{
				for (i = 0; arg[i].optc != 0x00; ++i)
				{
					if (strcmp(av[n], arg[i].optc) == 0 || strcmp(av[n], arg[i].optl) == 0)
					{
						if (ac - n - 1 >= arg[i].nbrarg)
						{
							arg[i].func(arg[i].nbrarg, &(av[n + 1]));
							n += arg[i].nbrarg;
						}
					}
				}
			}
		}
	}

	run_server();

	#ifdef DEBUG
		dprintf(1, "=> Exit daemon\n");
	#endif
	return (0);
}
