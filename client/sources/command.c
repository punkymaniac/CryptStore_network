#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "glob/core.h"
#include "glob/host_struct.h"
#include "client.h"
#include "command.h"

static void						close_socket_daemon(int sig)
{
	(void)sig;
	dprintf(2, "failed : daemon is killed\n");
	#ifdef DEBUG
		dprintf(1, "=> Exit client\n");
	#endif
	exit(1);
}

static struct cmd			*st_get_cmd(void)
{
	/*
	array doit avoir le meme ordre que array dans st_get_cmd du fichier
	daemon/sources/command.c
	*/
	static struct cmd		array[10] = {
		{"addhost", &c_addhost},
		{"listhost", &c_listhost},
		{0x00, 0x00}
	};
	return (array);
}

static short					st_search_cmd(char *str)
{
	struct cmd			*acmd;
	short						pos;

	acmd = st_get_cmd();
	for (pos = 0; acmd[pos].name != 0x00; ++pos)
	{
		if (strcmp(str, acmd[pos].name) == 0)
			return (pos);
	}
	return (-1);
}

void					exec_cmd(char *cmd, char **arg)
{
	struct cmd	*acmd;
	short				idcmd;

	acmd = st_get_cmd();
	if (cmd[0] == 0x00)
		return ;
	idcmd = st_search_cmd(cmd);
	if (idcmd == -1)
		dprintf(2, "%s: command not found\n", cmd);
	else
	{
		#ifdef DEBUG
			dprintf(1, "Execute command: %s\n", cmd);
		#endif
		acmd[idcmd].func(idcmd, arg);
		signal(SIGPIPE, close_socket_daemon);
	}
}

void					c_addhost(short idcmd, char **arg)
{
	struct infod				*infod;
	char								data[100] = {0};
	short								size;
	struct in_addr			ip;
	unsigned short			port;
	struct sockaddr_in	sa;
	long								val;
	char								*endptr = 0x00;

	size = 0;
	infod = info_daemon();
	if (arg == 0x00 || arg[0] == 0x00 || arg[1] == 0x00)
	{
		dprintf(1, "addhost <ip> <port>\n");
		return ;
	}
	// check format address ipv4
	if (inet_pton(AF_INET, arg[0], &(sa.sin_addr)) == 0)
	{
		dprintf(1, "Bad ip : %s\n", arg[0]);
		return ;
	}
	errno = 0;
	val = strtol(arg[1], &endptr, 10);
	if (errno != 0 || *endptr != 0x00)
	{
		dprintf(2, "Bap port: %s\n", arg[1]);
		return ;
	}
	if (val <= 0 || val > MAX_PORT)
	{
		dprintf(2, "Bap port: %s\n", arg[1]);
		return ;
	}
	port = (unsigned short)val;
	inet_aton(arg[0], &ip);
	memcpy(data, &idcmd, sizeof(idcmd));
	size += sizeof(idcmd);
	memcpy(&data[size], &ip, sizeof(struct in_addr));
	size += sizeof(struct in_addr);
	memcpy(&data[size], &port, sizeof(unsigned short));
	size += sizeof(unsigned short);
	if (send(infod->socket, data, size, 0) == -1)
		perror("send error");
}

void					c_listhost(short idcmd, char **arg)
{
	(void)arg;
	struct infod				*infod;
	char								data[10] = {0};
	short								size;
	fd_set							sockfd;
	int									recvo;
	struct timespec			timeout;
	struct host					host;
	char								lastconnect[100];
	int									lasttime;
	struct tm						*local;

	infod = info_daemon();
	size = 0;
	timeout.tv_sec = 0;
	timeout.tv_nsec = NSEC_REFRESH;
	memcpy(data, &idcmd, sizeof(idcmd));
	size += sizeof(idcmd);
	if (send(infod->socket, data, size, 0) == -1)
		perror("send error");
	while (1)
	{
		FD_ZERO(&sockfd);
		FD_SET(infod->socket, &sockfd);
		if (pselect(infod->socket + 1, &sockfd, 0x00, 0x00, &timeout, 0x00) == -1)
			perror("ERROR pselect client");
		if (FD_ISSET(infod->socket, &sockfd))
		{
			recvo = recv(infod->socket, &host, sizeof(struct host), 0x00);
			if (recvo == 1 && ((char *)&host)[0] == '\0')
				return ;
			if (recvo == 0)
			{
				dprintf(1, "ERROR\n");
			}
			else
			{
				// reuse data buffer for print opt host
				memset(data, '-', 3);
				data[3] = '\0';
				if ((host.opt & OPT_VALID) != 0)
					data[1] = 'V';
				local  = localtime(&host.last_connect);
				strftime(lastconnect, sizeof(lastconnect), "%m/%d/%Y", local);
				dprintf(1, "%s %s:%d %s ; ", \
					data, inet_ntoa(host.ip), host.port, host.hash);
				if (host.last_connect)
				{
					lasttime = (int)difftime(time(0x00), host.last_connect);
					lasttime = lasttime / (60 * 60 * 24);
					if (lasttime)
						dprintf(1, "%d days (%s)\n", lasttime, lastconnect);
					else
						dprintf(1, "today (%s)\n", lastconnect);
				}
				else
				{
					dprintf(1, "Never\n");
				}
			}
		}
	}
}
