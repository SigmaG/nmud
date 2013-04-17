#include <stdlib.h>

struct recv_info {
	int sock;
	struct windows* W;
};

void* recv_thread(void *arg) {

	struct recv_info* ri;
	ri = (struct recv_info *) arg;
	char *line;
int i = 0;
while (i < 10) {
i+=1;
//	while (1) {
		line = (char*) read_network_line(ri->sock);
		write_main(ri->W,line);
	}
}

int main() {

struct conf* cnf = (struct conf*) read_config();

initialize_windows();
struct windows* W;
W = (struct windows*) prepare_windows(cnf);

int mudsock = connect_to_mud(cnf);
if (mudsock == -1) {
	exit(-1);
}

pthread_t recv_pthr;

struct recv_info* ri;
ri = malloc(sizeof(struct recv_info));
ri->sock = mudsock;
ri->W = W;

pthread_create(&recv_pthr,NULL,&recv_thread,(void*) ri);
void *res;
pthread_join(recv_pthr,&res);

close_connection(mudsock);
sleep(2);
close_windows();
return 0;

}

