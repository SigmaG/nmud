#include <ncurses.h>
#include "config.h"

struct windows {
	WINDOW* m_win;
	WINDOW* t_win;
	WINDOW* r_win;
	WINDOW* b_win;
};

int initialize_windows();
struct windows* prepare_windows(struct conf* c);
int close_windows();
int write_main(struct windows* W, char* l);