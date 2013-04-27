#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "windows.h"

struct buffer {
	char** b;
	char** start;
	int fill;
	int lines;
};

struct win {
	WINDOW* w;
	struct buffer* buf;
	int top_line;
	int bottom_line;
	int scrolling;
};

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

struct win* create_window(int rows, int cols, int x, int y, int buf_size) {

	struct win* neww = malloc(sizeof(struct win));
	WINDOW* wi = newwin (rows,cols,x,y);

	neww->w = wi;

	scrollok(neww->w,TRUE);
	clearok(neww->w,TRUE);
	wrefresh(neww->w);

	neww->buf = malloc(sizeof(struct buffer));
	neww->buf->lines = buf_size;
	neww->buf->fill = 0;
	neww->buf->b = malloc(sizeof(char*)*buf_size);
	neww->buf->start = neww->buf->b;
	neww->top_line = 0;
	neww->bottom_line = 0;
	neww->scrolling = 0;

	return neww;
}

struct windows* prepare_windows(struct conf* cnf) {

	struct windows* W = malloc(sizeof(struct windows));

	int t_size;
	int r_size;
	int buf_size;
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
	if (cnf == NULL | cnf->buffer < 2*LINES) {
		buf_size = 2*LINES;
	} else {
		buf_size = cnf->buffer;
	}

	W->t_win = create_window(t_size+1,COLS,0,0,buf_size);
	W->r_win = create_window(LINES-t_size-1,r_size+1,t_size+1,COLS-r_size-1,buf_size);
	W->b_win = create_window(2,COLS-r_size-1,LINES-2,0,buf_size);
	W->m_win = create_window(LINES-t_size-3,COLS-r_size-1,t_size+1,0,buf_size);

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
	if (num == 0) return 0;
	if (num > 0) { //upwards
	fprintf(stderr,"UPWARDS start: bottom_line %d - wi->buf->fill %d - scrolling %d - top_line %d\n",wi->bottom_line,wi->buf->fill,wi->scrolling,wi->top_line);
		if (wi->top_line > 0) {
			wi->top_line -= num;
			if (wi->top_line < 0) wi->top_line = 0;
			wi->bottom_line = wi->top_line + getmaxy(wi->w) - 1;
			if (wi->bottom_line > wi->buf->fill) wi->bottom_line = wi->buf->fill;
			wi->scrolling = 1;
		} else {
			return 0;
		}
	fprintf(stderr,"UPWARDS end: bottom_line %d - wi->buf->fill %d - scrolling %d - top_line %d\n",wi->bottom_line,wi->buf->fill,wi->scrolling,wi->top_line);
	} else { // downwards
	fprintf(stderr,"DOWNWARDS start: bottom_line %d - wi->buf->fill %d - scrolling %d - top_line %d\n",wi->bottom_line,wi->buf->fill,wi->scrolling,wi->top_line);
		if (wi->scrolling == 1) {
			if (wi->bottom_line - num > wi->buf->fill) {
				wi->bottom_line = wi->buf->fill;
				wi->scrolling = 0;
			} else {
				wi->bottom_line -= num;
				if (wi->bottom_line == wi->buf->fill) wi->scrolling = 0;
			}
			wi->top_line = wi->bottom_line - getmaxy(wi->w) + 1;
			if (wi->top_line < 0) {
				wi->scrolling = 0;
				wi->top_line = 0;
			}
		} else {
			return 0;
		}
	fprintf(stderr,"DOWNWARDS end: bottom_line %d - wi->buf->fill %d - scrolling %d - top_line %d\n",wi->bottom_line,wi->buf->fill,wi->scrolling,wi->top_line);
	}
	int i;
/*	char* empty_line;
	empty_line = malloc((getmaxx(wi->w)+2)*sizeof(char));
	for (i=0 ; i < getmaxx(wi->w) ; i++) {
		empty_line[i] = ' ';
	}
	empty_line[getmaxx(wi->w)] = '\n';
	empty_line[getmaxx(wi->w)+1] = '\0';
	mvwprintw(wi->w,0,0,"");
	if (wi->bottom_line - wi->top_line < getmaxy(wi->w) - 1) {
		for (i=0 ; i < wi->bottom_line - wi->top_line - getmaxy(wi->w) ; i++) {
			wprintw(wi->w,"%s",empty_line);
		}
	}*/
	char** cur;
	for (i = wi->top_line ; i < wi->bottom_line ; i++) {
		if (wi->buf->start + i > wi->buf->b + wi->buf->lines) {
			cur = wi->buf->start + i - wi->buf->lines;
		} else {
			cur = wi->buf->start + i;
		}
		wprintw(wi->w,"%s",*cur);
	}
	wrefresh(wi->w);
}

int scroll_main(struct windows* W, int num) {
	scroll_win(W->m_win,num);
}

int write_win(struct win* wi, char* l) {

	if (wi->buf->fill < wi->buf->lines) { //still filling the buffer
		char** li = wi->buf->b;
		li += wi->buf->fill;
		*li = l;
		wi->buf->fill++;
	} else { //buffer full, overwrite
		*wi->buf->start = l;
		if (wi->buf->start - wi->buf->b == wi->buf->lines - 1) { //end of buffer
			wi->buf->start = wi->buf->b;
		} else {
			wi->buf->start += 1;
		}
	}
	
	if (wi->buf->fill < getmaxy(wi->w) - 1) { //still filling the window
		wi->top_line = 0;
		wi->bottom_line = wi->buf->fill;
		mvwprintw(wi->w,getmaxy(wi->w)-1,0,"%s",l);
	} else { //window full
		if (wi->scrolling == 1) {// are we scrolling?
			//more text in buffer but window doesn't move, sooo
			wi->top_line--;
			wi->bottom_line--;
//			mvwprintw(wi->w,getmaxy(wi->w)-1,0,"--MORE--\n");
		} else { //not scrolling, just print the line at the bottom
			if (wi->buf->fill < wi->buf->lines) {//buffer not full
				wi->top_line++;
				wi->bottom_line++;
			} 
			mvwprintw(wi->w,getmaxy(wi->w)-1,0,"%s",l);
		}
	}
	wrefresh(wi->w);
	fprintf(stderr,"FIRST LINE in buffer #%d: %s\n",wi,*wi->buf->start);
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