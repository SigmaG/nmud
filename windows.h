#include <ncurses.h>
#include "config.h"

struct windows {
	struct win* m_win;
	struct win* t_win;
	struct win* r_win;
	struct win* b_win;
};

int initialize_windows();
struct windows* prepare_windows(struct conf* c);
int close_windows();
int write_main(struct windows* W, char* l);
int write_top(struct windows* W, char* l);
int write_right(struct windows* W, char* l);
int write_bottom(struct windows* W, char* l);
int clear_main(struct windows* W);
int clear_top(struct windows* W);
int clear_right(struct windows* W);
int clear_bottom(struct windows* W);
int scroll_main(struct windows* W, int num);