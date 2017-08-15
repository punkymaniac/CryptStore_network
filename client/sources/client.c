#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "glob/core.h"
#include "client.h"

static char			**st_parse(char *buf)
{
	int				i;
	short			word;
	short			posarg;
	char			**arg;

	posarg = 0;
	word = 0;
	arg = (char **)malloc(sizeof(char *) * 100);
	for (i = 0; buf[i]; ++i)
	{
		if (word == 0 && buf[i] != 0x20 && buf[i] != 0x09 && buf[i] != 0x0B)
		{
			word = 1;
			arg[posarg] = &buf[i];
			++posarg;
			if (posarg == 99)
				break;
		}
		else if (word == 1 && (buf[i] == 0x20 || buf[i] == 0x09 || buf[i] == 0x0B))
		{
			buf[i] = 0x00;
			word = 0;
		}
	}
	buf[i - 1] = 0x00;
	arg[posarg] = 0x00;
	return (arg);
}

void			run_shell(void)
{
	char		buf[SIZE_BUFFER];
	int			nbr;
	char		**arg;

	memset(buf, 0, SIZE_BUFFER);
	while (1)
	{
		write(1, "> ", 2);
		if ((nbr = read(0, buf, SIZE_BUFFER)) != -1)
		{
			buf[nbr] = 0x00;
			arg = st_parse(buf);
			run_cmd(arg[0], &arg[1]);
			free(arg);
		}
	}
}

int				connect_daemon(void)
{
	int										sock;
	struct sockaddr_un		sun;

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
	send(sock, "init ", 5, 0x00);
	return (sock);
}
