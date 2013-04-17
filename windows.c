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

WINDOW* twin = newwin (t_size+1,COLS,0,0);
W->t_win = twin;
mvwhline(W->t_win,t_size,0,ACS_HLINE,COLS-r_size-1);
mvwaddch(W->t_win,t_size,COLS-r_size-1,ACS_TTEE);
mvwhline(W->t_win,t_size,COLS-r_size,ACS_HLINE,r_size+1);
scrollok(W->t_win,TRUE);
clearok(W->t_win,TRUE);
wrefresh(W->t_win);

WINDOW* rwin = newwin (LINES - t_size-1,r_size+1,t_size+1,COLS-r_size -1);
W->r_win = rwin;
mvwvline(W->r_win,0,0,ACS_VLINE,LINES - t_size-3);
mvwaddch(W->r_win,LINES-t_size-3,0,ACS_RTEE);
mvwaddch(W->r_win,LINES-t_size-2,0,ACS_VLINE);
scrollok(W->r_win,TRUE);
clearok(W->r_win,TRUE);
wrefresh(W->r_win);

WINDOW* bwin = newwin (2, COLS-r_size-1,LINES-2,0);
W->b_win = bwin;
mvwhline(W->b_win,0,0,ACS_HLINE,COLS-r_size-1);
scrollok(W->b_win,TRUE);
clearok(W->b_win,TRUE);
wrefresh(W->b_win);

WINDOW* mwin = newwin(LINES - t_size - 3,COLS-r_size-1,t_size+1,0);
W->m_win = mwin;
scrollok(W->m_win,TRUE);
clearok(W->m_win,TRUE);
wrefresh(W->m_win);

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

int write_win(WINDOW* win, char*l) {
	mvwprintw(win,getmaxy(win)-1,0,"%s",l);
	wrefresh(win);
	return 0;
}

int write_main(struct windows* W, char* l) {
	return write_win(W->m_win,l);
}

int write_top(struct windows* W, char* l) {
	return write_win(W->t_win,l);
}

int write_right(struct windows* W, char* l) {
	return write_win(W->r_win,l);
}

int write_bottom(struct windows* W, char* l) {
	return write_win(W->b_win,l);
}

int clear_window(WINDOW* win) {
	wclear(win);
	wrefresh(win);
	return 0;
}

int clear_bottom(struct windows* W) {
	clear_window(W->b_win);
}
int clear_top(struct windows* W) {
	clear_window(W->t_win);
}
int clear_right(struct windows* W) {
	clear_window(W->r_win);
}
int clear_main(struct windows* W) {
	clear_window(W->m_win);
}