#include <stdio.h>
#include <string.h>

struct conf {
	char* address;
	char* port;
	int r_size;
	int t_size;
	int buffer;
};

extern struct conf *read_config();