#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "glob/core.h"
#include "server.h"

int			create_p2p_socket(int port)
{
	int									empty;
	int									sock;
	struct protoent			*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname(PROTOCOL)) == 0x00)
	{
		dprintf(2, "ERROR getprotobyname: Bad protocol %s\n", PROTOCOL);
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

void			recv_new_connection(int socket)
{
	int										sockcli;
	int										cslen;
	struct sockaddr_in		csin;

	cslen = sizeof(csin);
	memset(&csin, 0, sizeof(csin));
	if ((sockcli = accept(socket, (struct sockaddr *)&csin, \
				(socklen_t *)&cslen)) == -1)
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
		dprintf(1, "Client connected: %s:%d\n", ipstr, csin.sin_port);
	}
	close(sockcli);
}
