#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "glob/core.h"
#include "server.h"
#include "config.h"
#include "localclient.h"
#include "p2p.h"
#include "p2p_host.h"

static void			stop_service_signal(int sig)
{
	(void)sig;
	#ifdef DEBUG
		dprintf(1, "=> Exit daemon\n");
	#endif
	exit(0);
}

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
	struct timespec			timeout;
	struct conf					*config;

	signal(SIGTERM, stop_service_signal); // controle stop service
	config = get_config();
	sock = create_p2p_socket(config->listen_port);
	socku = create_unix_socket();
	init_hosts(config->host_address, config->listen_port);
	timeout.tv_sec = 0;
	timeout.tv_nsec = NSEC_REFRESH;
	FD_ZERO(&sockfd);
	FD_SET(sock, &sockfd);
	FD_SET(socku, &sockfd);
	maxsock = (sock > socku) ? sock : socku;
	while (1)
	{
		FD_ZERO(&rfds);
		rfds = sockfd;
		if (pselect(maxsock + 1, &rfds, 0x00, 0x00, &timeout, 0x00) == -1)
			fail_server("ERROR pselect");
		if (FD_ISSET(sock, &rfds)) // if connexion p2p
		{
			recv_new_connection(sock);
		}
		else if (FD_ISSET(socku, &rfds)) // if connexion client local
		{
			connect_local(socku);
		}
		control_client(&timeout);
	}
}
