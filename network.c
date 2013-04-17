#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "errors.h"

int connect_to_mud(struct conf* cnf) {

struct addrinfo hints;
struct addrinfo *result, *rp;
int mudsock;

memset(&hints,0,sizeof(struct addrinfo));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_ADDRCONFIG;
hints.ai_protocol = 0;
int s = getaddrinfo(cnf->address,cnf->port,&hints,&result);
if (s != 0) {
	display_error("getaddrinfo : %s\n", gai_strerror(s));
	return -1;
}

for (rp = result; rp != NULL; rp = rp->ai_next) {
	mudsock = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
	if (mudsock == -1) continue;
	if (connect(mudsock, rp->ai_addr, rp->ai_addrlen) != -1) break;
	close(mudsock);
}

if (rp == NULL) {
	display_error("Could not connect\n");
	return -1;
}

freeaddrinfo(result);
return mudsock;
//block or non block?
}

char* read_network_line(int sock) {
	int m = 256;
	char* sss = malloc(m*sizeof(char));
	while (1) {
		sss = realloc(sss,m*sizeof(char));
		int k;
		k = recv(sock,sss,m,MSG_PEEK);
		if (k == -1) {
			display_error("Error in reception\n");
			return NULL;
		}
		int nl = strcspn(sss,"\n");
		if (nl < k) {
			k = recv(sock,sss,nl+1,0);
			sss[nl+1]='\0';
			return sss;
		}
		if (k == m) {
			m *= 2;
		} else {
			k = recv(sock,sss,m,0);
			return sss;
		}
	}
}

int write_network_line(char* r, int sock) {
	write(sock, r, strlen(r));
}

int close_connection(int sock) {
	close(sock);
}


int n_main() {

struct conf* cnf = read_config();
int mudsock = connect_to_mud(cnf);

if (mudsock == -1) return 1;
write_network_line("tolsuatoheushtaso,euht\naoeu\n",mudsock);

char* test = read_network_line(mudsock);
printf("%s",test);

close(mudsock);
return 0;

}