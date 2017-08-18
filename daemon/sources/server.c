#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "glob/core.h"
#include "localclient.h"
#include "p2p.h"

void			fail_server(char *msgfail)
{
	#ifdef DEBUG
		dprintf(1, "Fail run serveur\n");
	#endif
	if (msgfail)
		perror(msgfail);
	exit(-1);
}

void				run_server(void)
{
	fd_set							sockfd;
	fd_set							rfds;
	int									sock;
	int									socku;
	int									maxsock;
	struct timeval			time = {0, 0};

	sock = create_p2p_socket(DEFAULT_PORT);
	socku = create_unix_socket();
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
		if (FD_ISSET(sock, &rfds)) // if connexion p2p
		{
			recv_new_connection(sock);
		}
		else if (FD_ISSET(socku, &rfds)) // if connexion client local
		{
			connect_local(socku);
		}
		control_client();
	}
}
