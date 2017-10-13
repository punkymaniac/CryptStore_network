#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "glob/core.h"
#include "client.h"

int			main(int ac, char **av)
{
	#ifdef DEBUG
		dprintf(1, "=== MODE DEBUG ===\n");
		dprintf(1, "=> Run client\n");
	#endif

	int							sockd;
	struct infod		*infod;

	if (ac == 1)
	{
		// TODO print result exec cmd help <- feature
		return (0);
	}
	infod = info_daemon();
	if ((sockd = connect_daemon()) != -1)
	{
		infod->socket = sockd;
		exec_cmd(av[1], &(av[2]));
	}

	#ifdef DEBUG
		dprintf(1, "=> Exit client\n");
	#endif
	return (0);
}

int				connect_daemon(void)
{
	int										sock;
	struct sockaddr_un		sun;
	int										nrecv;

	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, UNIX_SOCKET_PATH);
	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("ERROR socket");
		return (-1);
	}
	if (connect(sock, (struct sockaddr *)&sun, sizeof(sun)) == -1)
	{
		perror("ERROR connect");
		return (-1);
	}
	nrecv = recv(sock, 0x00, 1, 0x00);
	if (nrecv != 0)
	{
		return (sock);
	}
	dprintf(1, "One client is already connected\n");
	close(sock);
	return (-1);
}

struct infod		*info_daemon(void)
{
	static struct infod		info = {0};

	return (&info);
}
