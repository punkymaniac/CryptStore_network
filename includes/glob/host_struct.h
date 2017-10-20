#ifndef HOST_STRUCT_H
#define HOST_STRUCT_H

#define OPT_DEFAULT		0x0000
#define OPT_CURRENT		0x0001
#define OPT_VALID			0x0002

#define NEVER 0

struct host
{
	unsigned short		opt;
	char							hash[20];
	struct in_addr		ip;
	unsigned short		port;
	time_t						last_connect;
	struct host				*next;
};
#endif
