/*
  graphics.c
  Curses interface
  -------------------------------------
*/
#include "graphics.h"

static struct visor{
	int row;
	int col;
	int rows;
	int cols;
}visor;

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
void g_vputc(int row, int col, char c, int fgColor, int bgColor){
	int vrow, vcol;
	vrow = row - visor.row;
	vcol = col - visor.col;
	if(vrow >= 0 && vrow < visor.rows){
		if(vcol >= 0 && vcol < visor.cols){
			g_putc(vrow, vcol, c, fgColor, bgColor);
		}
	}
}
void g_vset(int row, int col, int rows, int cols){
	visor.row = row;
	visor.col = col;
	visor.rows = rows;
	visor.cols = cols;
	g_repaint();
}
void g_drawstatus(){

}
void g_drawhelp(){

}
void g_drawfinder(int row, int col, char aspect, int bg, int fg){
	g_vputc(row, col, aspect,fg,bg); 
}
void g_drawvehicle(int row, int col, char aspect, int bg, int fg, int selected){
	g_vputc(row, col, aspect,selected?bg:fg, selected?fg:bg); 
}
void g_drawcross(int row, int col, char aspect, int fg, int bg, int selected){
	g_vputc(row, col, aspect,selected?bg:fg, selected?fg:bg); 
}
void g_drawrail(int row, int col, char aspect, int fg, int bg){
	g_vputc(row, col, aspect, fg, bg); 
}
void g_clearrail(int row, int col, char aspect, int fg, int bg){
	g_vputc(row, col, ' ', fg, bg); 
}
