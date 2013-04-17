#include "config.h"
#include <stdlib.h>
#include <stdio.h>

struct conf* read_config() {

struct conf* c = malloc(sizeof(struct conf));

FILE* cnf;

cnf = fopen("/home/sigma/dev/nmud/nmud.conf","r");
if (cnf == NULL) {
	return NULL;
}

c->port = 0;
c->r_size = 0;
c->t_size = 0;
c->address = NULL;
while (1) {
	char* l = NULL;
	size_t k;
	ssize_t n = getline(&l,&k,cnf);

	if (n == -1) break;
	
	char x = l[0];
	int i = 0;
	char* attr;
	char* value;
	int ieq = -1;
	while (1) {
		if (x == '#' | x == '\n' | x == '\0') {
		if (ieq > -1) {
			value = strndup(l+ieq+1,i-ieq-1);
			}
			break;
		}
		if (x == '=') {
			attr = strndup(l,i);
			ieq = i;
		}
		i++;
		x = l[i];
	}
	if (strcmp(attr,"address") == 0) {
		c->address = value;
	}
	if (strcmp(attr,"port") == 0) {
		c->port = value;
	}
	if (strcmp(attr,"r_win") == 0) {
		c->r_size = atoi(value);
	}
	if (strcmp(attr,"t_win") == 0) {
		c->t_size = atoi(value);
	}
	
}
return c;

}

int c_main() {

struct conf* c = read_config();
	printf("ADDRESS: %s\n",c->address);
	printf("PORT: %s\n",c->port);
	printf("RIGHT WINDOW SIZE: %d\n",c->r_size);
	printf("TOP WINDOW SIZE: %d\n",c->t_size);
return 0;

}