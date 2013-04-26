#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "windows.h"

int initialize_windows() {

initscr();
start_color();
raw();
noecho();
nonl();
intrflush(stdscr,FALSE);
keypad(stdscr,TRUE);
return 0;

}

int redraw_borders(struct windows* W) {

int t_size = getmaxy(W->t_win->w)-1;
int r_size = getmaxx(W->r_win->w)-1;

mvwhline(W->t_win->w,t_size,0,ACS_HLINE,COLS-r_size-1);
mvwaddch(W->t_win->w,t_size,COLS-r_size-1,ACS_TTEE);
mvwhline(W->t_win->w,t_size,COLS-r_size,ACS_HLINE,r_size+1);
wrefresh(W->t_win->w);

mvwvline(W->r_win->w,0,0,ACS_VLINE,LINES - t_size-3);
mvwaddch(W->r_win->w,LINES-t_size-3,0,ACS_RTEE);
mvwaddch(W->r_win->w,LINES-t_size-2,0,ACS_VLINE);
wrefresh(W->r_win->w);

mvwhline(W->b_win->w,0,0,ACS_HLINE,COLS-r_size-1);
wrefresh(W->b_win->w);

wrefresh(W->m_win->w);
return 0;
}


int close_windows() {

endwin();
return 0;

}

struct windows* prepare_windows(struct conf* cnf) {

struct windows* W = malloc(sizeof(struct windows));

int t_size;
int r_size;
if (cnf == NULL | cnf->t_size == 0) {
t_size = 6;
} else {
t_size = cnf->t_size;
}
if (cnf == NULL | cnf->t_size == 0) {
r_size = 25;
} else {
r_size = cnf->r_size;
}


struct win* tw = malloc(sizeof(struct win));
WINDOW* twin = newwin (t_size+1,COLS,0,0);

tw->w = twin;
W->t_win = tw;

scrollok(W->t_win->w,TRUE);
clearok(W->t_win->w,TRUE);
wrefresh(W->t_win->w);


WINDOW* rwin = newwin (LINES - t_size-1,r_size+1,t_size+1,COLS-r_size -1);
struct win* rw = malloc(sizeof(struct win));

rw->w = rwin;
W->r_win = rw;

scrollok(W->r_win->w,TRUE);
clearok(W->r_win->w,TRUE);
wrefresh(W->r_win->w);


WINDOW* bwin = newwin (2, COLS-r_size-1,LINES-2,0);
struct win* bw = malloc(sizeof(struct win));

bw->w = bwin;
W->b_win = bw;

scrollok(W->b_win->w,TRUE);
clearok(W->b_win->w,TRUE);
wrefresh(W->b_win->w);


WINDOW* mwin = newwin(LINES - t_size - 3,COLS-r_size-1,t_size+1,0);
struct win* mw = malloc(sizeof(struct win));

mw->w = mwin;
W->m_win = mw;

scrollok(W->m_win->w,TRUE);
clearok(W->m_win->w,TRUE);
wrefresh(W->m_win->w);

redraw_borders(W);
return W;
}

/*
int write_main(struct windows* W, char* l) {
	char* l2; char* l0; char* ls; ls = malloc(getmaxx(W->m_win)+2); l2 = 
	malloc((strlen(l)+1)*sizeof(char)); l0 = l2; strncpy(l2,l,strlen(l)+1); 
	while (1) {
			strncpy(ls,l2,getmaxx(W->m_win));
			ls[getmaxx(W->m_win)] = '\n';
			ls[getmaxx(W->m_win)] = '\0';
			mvwprintw(W->m_win,getmaxy(W->m_win)-1,0,"%s",ls);
			l2 += getmaxx(W->m_win)*sizeof(char);
		} else {
			mvwprintw(W->m_win,getmaxy(W->m_win)-1,0,"%s",l2);
			break;
		}
	}
	wrefresh(W->m_win);
	free(l0);
	free(ls);
}
*/

int scroll_win(struct win* wi, int num) {
	wscrl(wi->w,num);
}

int scroll_main(struct windows* W, int num) {
	scroll_win(W->m_win,num);
}

int write_win(struct win* wi, char*l) {
	mvwprintw(wi->w,getmaxy(wi->w)-1,0,"%s",l);
	wrefresh(wi->w);
	return 0;
}

int write_main(struct windows* W, char* l) {
	int i = write_win(W->m_win,l);
	redraw_borders(W);
	return i;
}

int write_top(struct windows* W, char* l) {
	int i = write_win(W->t_win,l);
	redraw_borders(W);
	return i;
}

int write_right(struct windows* W, char* l) {
	int i = write_win(W->r_win,l);
	redraw_borders(W);
	return i;
}

int write_bottom(struct windows* W, char* l) {
	int i = write_win(W->b_win,l);
	redraw_borders(W);
	return i;
}

int clear_window(struct win* wi) {
	wclear(wi->w);
	wrefresh(wi->w);
	return 0;
}

int clear_bottom(struct windows* W) {
	clear_window(W->b_win);
	redraw_borders(W);
}
int clear_top(struct windows* W) {
	clear_window(W->t_win);
	redraw_borders(W);
}
int clear_right(struct windows* W) {
	clear_window(W->r_win);
	redraw_borders(W);
}
int clear_main(struct windows* W) {
	clear_window(W->m_win);
	redraw_borders(W);
}