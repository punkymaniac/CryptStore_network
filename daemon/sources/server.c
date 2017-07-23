#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "glob/core.h"

static int	create_socket(int port)
{
	int									empty;
	int									sock;
	struct protoent			*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname(PROTOCOL)) == 0x00)
	{
		dprintf(2, "ERROR getprotbyname: Bad protocol %s\n", PROTOCOL);
		exit(-1);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((sock = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		perror("ERROR socket");
		exit(-1);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &empty, sizeof(empty)) == -1)
	{
		perror("ERROR setsockopt");
		exit(-1);
	}
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		perror("ERROR bind");
		exit(-1);
	}
	if (listen(sock, 10) == -1)
	{
		perror("ERROR listen");
		exit(-1);
	}
	return (sock);
}

void				run_server(void)
{
	fd_set							rfds;
	struct timeval			time = {0, 0};
	int									sock;
	int									sockcli;
	unsigned int				cslen;
	struct sockaddr_in	csin;

	sock = create_socket(DEFAULT_PORT);
	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(sock, &rfds);
		if (select(sock + 1, &rfds, NULL, NULL, &time) == -1)
		{
			perror("ERROR select");
			exit(-1);
		}
		if (FD_ISSET(sock, &rfds))
		{
			cslen = sizeof(csin);
			memset(&csin, 0, sizeof(csin));
			if ((sockcli = accept(sock, (struct sockaddr *)&csin, &cslen)) == -1)
			{
				#ifdef DEBUG
					perror("ERROR accept");
				#endif
			}
			else
			{
				// TODO gestion temporaire acceptation client
				char		ipstr[INET_ADDRSTRLEN];
				if (inet_ntop(AF_INET, &csin.sin_addr, ipstr, sizeof(ipstr)) == 0x00)
				{
					#ifdef DEBUG
						perror("ERROR inet_ntop");
					#endif
				}
				dprintf(1, "Client connected: %s:%d\n", ipstr, ntohs(csin.sin_port));
			}
			close(sockcli);
		}
	}
	close(sock);
}
