#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "glob/core.h"
#include "server.h"
#include "argument.h"
#include "config.h"

static void								arg_port(int nbrarg, char **av)
{
	(void)nbrarg;
	struct conf						*config;
	char									*end;
	long int							port;

	config = get_config();
	errno = 0;
	port = strtol(av[0], &end, 10);
	if (errno == ERANGE || port >= MAX_PORT || port <= 0 || end[0] != '\0')
	{
		dprintf(1, "[ERROR] bad port: %s\n", av[0]);
		return ;
	}
	config->listen_port = port;
	#ifdef DEBUG
		dprintf(1, "[CONFIG] listen port: %d\n", config->listen_port);
	#endif
}

static void							arg_address(int nbrarg, char **av)
{
	(void)nbrarg;
	struct conf					*config;
	int									size;
	struct sockaddr_in	sa;

	config = get_config();
	size = strlen(av[0]);
	// check valid format ipv4
	if (inet_pton(AF_INET, av[0], &(sa.sin_addr)) == 0)
	{
		dprintf(2, "[CONFIG] error address is not valid\n");
		return ;
	}
	memcpy(config->host_address, av[0], size);
	config->host_address[size] = 0x00;
	#ifdef DEBUG
		dprintf(1, "[CONFIG] Host address: %s\n", config->host_address);
	#endif
}

static struct argument		*st_get_arg(void)
{
	static struct argument	array[10] = {
		{"-p", "--port", 1, arg_port},
		{"-a", "--address", 1, arg_address},
		{0x00, 0x00, 0, 0x00}
	};
	return (array);
}

struct conf						*get_config(void)
{
	static struct conf	config = {
			.listen_port = DEFAULT_LISTEN_PORT,
			.host_address = DEFAULT_HOST_ADDRESS
	};
	return (&config);
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
