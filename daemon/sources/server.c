#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "glob/core.h"

static void	fail_server(char *msgfail)
{
	#ifdef DEBUG
		dprintf(1, "Fail run serveur\n");
	#endif
	if (msgfail)
		perror(msgfail);
	exit(-1);
}

static int	create_unix_socket(void)
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

static int	create_socket(int port)
{
	int									empty;
	int									sock;
	struct protoent			*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname(PROTOCOL)) == 0x00)
	{
		dprintf(2, "ERROR getprotbyname: Bad protocol %s\n", PROTOCOL);
		fail_server(0x00);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
		fail_server("ERROR socket");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &empty, sizeof(empty)) == -1)
		fail_server("ERROR setsockopt");
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
		fail_server("ERROR bind");
	if (listen(sock, 10) == -1)
		fail_server("ERROR listen");
	return (sock);
}

void				run_server(void)
{
	fd_set							rfds;
	fd_set							sockfd;
	struct timeval			time = {0, 0};
	int									sock;
	int									sockcli;
	int									socku;
	int									sockucli;
	unsigned int				cslen;
	struct sockaddr_in	csin;
	struct sockaddr_un	csun;
	int									maxsock;

	sock = create_socket(DEFAULT_PORT);
	socku = create_unix_socket();
	sockucli = 0;
	FD_ZERO(&sockfd);
	FD_SET(sock, &sockfd);
	FD_SET(socku, &sockfd);
	maxsock = (sock > socku) ? sock : socku;
	while (1)
	{
		FD_ZERO(&rfds);
		rfds = sockfd;
		if (select(maxsock + 1, &rfds, 0x00, 0x00, &time) == -1)
			fail_server("ERROR select");
		if (FD_ISSET(sock, &rfds))
		{
			// if connexion p2p
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
		else if (FD_ISSET(socku, &rfds))
		{
			// if connexion client local
			cslen = sizeof(csun);
			memset(&csun, 0, sizeof(cslen));
			if ((sockcli = accept(socku, (struct sockaddr *)&csun, &cslen)) == -1)
			{
				#ifndef DEBUG
					perror("ERROR accept local");
				#endif
			}
			if (sockucli == 0)
			{
				dprintf(1, "Client local connected\n");
				sockucli = sockcli;
				maxsock = (maxsock > sockucli) ? maxsock : sockucli;
				FD_SET(sockucli, &sockfd);
				send(sockucli, "", 1, 0x00);
			}
			else
			{
				close(sockcli);
			}
		}
		else if (FD_ISSET(sockucli, &rfds))
		{
			// if reception donne client local
			char			b[100] = {0};
			int				r = 0;
			r = recv(sockucli, &b, 100, 0x00);
			if (r == 0)
			{
				dprintf(1, "Client local disconected\n");
				FD_CLR(sockucli, &sockfd);
				close(sockucli);
				sockucli = 0;
			}
			else
			{
				b[r] = 0x00;
				dprintf(1, "RECV: %d [%s]\n", r, b);
			}
		}
	}
	close(sock);
}
