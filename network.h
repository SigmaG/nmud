#include "config.h"

int connect_to_mud(struct conf* cnf);
char* read_network_line(int sock);
int write_network_line(char* r, int sock);
int close_connection(int sock);