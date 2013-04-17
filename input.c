//#include "windows.h"
#include "input.h"
#include <stdlib.h>

void* handle_alt_key(struct input_struct* is,int k) {
	char l[100];
	snprintf(l,99,"alt+%c combo\n",(char) k);
	write_top(is->W,l);
	return NULL;
}

void* handle_keypresses(void* arg) {
	struct input_struct* is = (struct input_struct*) arg;
	char* buf;
	int bsize = 256;
	buf = malloc(bsize*sizeof(char));
	int cur = 0;
	int k;
	while (1) {
		k = getch();
		if (((k >= 32) && (k <= 126)) ) {
			buf[cur] = (char) k;
			buf[cur+1] = '\0';
			if (cur + 1 == bsize) {
				bsize += 256;
				buf = realloc(buf,bsize*sizeof(char));
			}
			cur += 1;
		}
		if (k == 27) {//escape key
			int k2 = getch();
			handle_alt_key(is,k2);
		}
		if (k == 3) {//^C, temporary
			exit(0);
		}
		if (k == 9) { //handle TAB
			write_bottom(is->W,"TAAAAB");
		}
		if (k == KEY_BACKSPACE) { //backwards
			clear_bottom(is->W);
			cur -= 1;
			if (cur < 0) {
				cur = 0;
			}
			buf[cur] = '\0';
		}
		if (k == 13) { //newline
			clear_bottom(is->W);
			buf[cur] = '\n';
			buf[cur + 1] = '\0';
			write_network_line(buf,is->sock);
			cur = 0;
			buf[0] = '\0';
		}
		
		write_bottom(is->W, buf);
	}
}

