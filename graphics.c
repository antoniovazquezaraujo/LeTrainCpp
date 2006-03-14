/*
  graphics.c
  Curses interface
  -------------------------------------
*/
#include "graphics.h"
void g_init(){
	int n=0, m=0;
	(void) initscr();      /* initialize the curses library */
	keypad(stdscr, TRUE);  /* enable keyboard mapping */
	(void) nonl();         /* tell curses not to do NL->CR/NL on output */
	(void) cbreak();       /* take input chars one at a time, no wait for \n */
	(void) noecho();       /* don't echo input */
	(void) curs_set(0);    /* no cursor */ 
    if (has_colors()) {
		start_color();
		for(n = 0; n<8; n++){
			for(m=0;m<8;m++){
				init_pair(n*8+m, n, m);
			}
		}
	}
	// Separar esto de aqui.
	/* 
	for(n = 0; n<MAX_COLS; n++){
		mvaddch(START_ROW-1,n+START_COL,'-');
	}
	for(n = 0; n<MAX_COLS; n++){
		mvaddch(START_ROW+MAX_ROWS,n+START_COL,'-');
	}
	for(m = 0;m<MAX_ROWS; m++){
		mvaddch(START_ROW+m,START_COL-1,'|');
		mvaddch(START_ROW+m,START_COL+ MAX_COLS,'|');
	}
	*/
}
void g_reset(){
    endwin();
}
int g_getkey(){
	if(halfdelay(1) != ERR){
		return getch();  
	}else{
		return -1;
	}
}
void g_putc(int row, int col, char c, int fgColor, int bgColor){
	attron(COLOR_PAIR(fgColor*8 + bgColor));
	attron(WA_BOLD);
	mvaddch(row,col,c);
	attroff(WA_BOLD);
	attroff(COLOR_PAIR(fgColor*8 + bgColor));
}
void g_repaint(){
	refresh();
}
