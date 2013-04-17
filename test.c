#include <stdlib.h>
#include "input.h"

struct recv_info {
	int sock;
	struct windows* W;
};

void* recv_thread(void *arg) {

	struct recv_info* ri;
	ri = (struct recv_info *) arg;
	char *line;
int i = 0;
//while (i < 10) {
//i+=1;
	while (1) {
		line = (char*) read_network_line(ri->sock);
		write_main(ri->W,line);
	}
}

int main() {

/*int z = 0;
while (z < 256) {
z++;
	printf("%i - %c\n",z,(char) z);
}
exit(0);*/
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

pthread_t send_pthr;
struct input_struct* is;
is = malloc(sizeof(struct input_struct));
is->W = W;
is->sock = mudsock;

pthread_create(&send_pthr,NULL,&handle_keypresses,(void*) is);

pthread_join(recv_pthr,&res);





close_connection(mudsock);
sleep(2);
close_windows();


return 0;

}

