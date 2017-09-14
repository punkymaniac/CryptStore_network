#ifndef ARGUMENT_H
#define ARGUMENT_H

struct argument
{
	char						*optc;
	char						*optl;
	unsigned short	nbrarg;
	void						(*func)(int nbrarg, char **av);
};

#define SEPARATOR_OPTION		'-'

#endif
