#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

#include "glob/core.h"
#include "server.h"

static int		*get_socket(void)
{
	static int	socket = 0;

	return (&socket);
}

int			create_unix_socket(void)
{
	int									sock;
	struct sockaddr_un	sun;

	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, UNIX_SOCKET_PATH);
	unlink(UNIX_SOCKET_PATH);
	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
		fail_server("ERROR socket local");
	if (bind(sock, (struct sockaddr *)&sun, sizeof(sun)) == -1)
		fail_server("ERROR bind local");
	if (listen(sock, 1) == -1)
		fail_server("ERROR listen local");
	return (sock);
}

static void			recv_local(void)
{
	int				*sockucli;
	char			b[100] = {0};
	int				r = 0;

	sockucli = get_socket();
	r = recv(*sockucli, &b, 100, 0x00);
	if (r == 0)
	{
		dprintf(1, "Client local disconected\n");
		close(*sockucli);
		*sockucli = 0;
	}
	else
	{
		b[r] = 0x00;
		dprintf(1, "RECV: %d [%s]\n", r, b);
	}
}

void			connect_local(int socket)
{
	int										*sockucli;
	int										sockcli;
	int										cslen;
	struct sockaddr_un		csun;

	sockucli = get_socket();
	cslen = sizeof(csun);
	memset(&csun, 0, sizeof(cslen));
	if ((sockcli = accept(socket, (struct sockaddr *)&csun, \
				(socklen_t *)&cslen)) == -1)
	{
		#ifndef DEBUG
			perror("ERROR accept local");
		#endif
	}
	if (*sockucli == 0)
	{
		dprintf(1, "Client local connected\n");
		*sockucli = sockcli;
		send(*sockucli, "", 1, 0x00);
	}
	else
	{
		close(sockcli);
	}
}

void			control_client(void)
{
	static fd_set						sockfd;
	static struct timeval		time = {0, 0};
	int											*sockucli;

	sockucli = get_socket();
	FD_ZERO(&sockfd);
	FD_SET(*sockucli, &sockfd);
	if (select((*sockucli) + 1, &sockfd, 0x00, 0x00, &time) == -1)
	{	
		perror("ERROR select client local");
	}
	if (FD_ISSET(*sockucli, &sockfd))
	{
		recv_local();
	}
}
