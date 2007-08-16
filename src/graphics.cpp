/*
  graphics.cpp
  Curses interface
  -------------------------------------
*/
#include <cstring>
#include "graphics.h"

static struct visor{
	int row;
	int col;
	int rows;
	int cols;
}visor, status;

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
void g_putstr(int row, int col, char *str, int fgColor, int bgColor){
	attron(COLOR_PAIR(fgColor*8 + bgColor));
	attron(WA_BOLD);
	mvaddstr(row,col,str);
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
void g_sputstr(int row, int col, char *str, int fgColor, int bgColor){
	int srow, scol;
	if(row >= 0 && row < status.rows){
		if(col >= 0 && col < status.cols){
			srow = row + status.row;
			scol = col + status.col;
			g_putstr(srow, scol, str, fgColor, bgColor);
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
void g_sset(int row, int col, int rows, int cols){
	status.row = row;
	status.col = col;
	status.rows = rows;
	status.cols = cols;
	g_repaint();
}
void g_drawVehicleStatus(int id, int row, int col, int dir, float imp, int power, int brakes, int link0id, int link1id, char aspect){
	int statusfg= 7;
	int statusbg = 0;
	//char data[status.cols];
	char *data = new char[status.cols];
	int line = 0;
	int r;
	for(r = 0; r<status.cols-6;r++){
		data[r] = ' ';
	}
	data[r] = '\0';
	for(r= 0; r< 10; r++){
		g_sputstr(r,5, data, statusfg, statusbg); 
	}

	sprintf(data, "Id: %d", id);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Row: %d", row);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Col: %d", col);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Dir: %d", dir);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Imp: %f", imp);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Power: %d", power);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Brakes: %d", brakes);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Link 0: %d", link0id);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Link 1: %d", link1id);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	sprintf(data, "Aspect: %c", aspect);
	g_sputstr(line++,5, data, statusfg, statusbg); 
	delete[] data;
}
void g_drawhelp(int mode){
	// quitar esto de aqui!
	enum{
		DRIVERS_MODE, 
		POINTS_MODE,
		TRAINS_MODE
	};
	int line = 12;
	int helpfg= 2;
	int helpbg = 0;
	char * data = new char [status.cols];
	int r;
	for(r = 0; r<status.cols-5;r++){
		data[r] = ' ';
	}
	data[r] = '\0';

	for(r = 12; r < 27; r++){
		g_sputstr(r,5, data, helpfg, helpbg); 
	}
	switch(mode){
	case DRIVERS_MODE:
		sprintf(data, "Mode: %s", "DRIVERS");
		break;
	case POINTS_MODE:
		sprintf(data, "Mode: %s", "POINTS");
		break;
	case TRAINS_MODE:
		sprintf(data, "Mode: %s", "TRAINS");
		break;
	}
	g_sputstr(line++,5, data, helpfg, helpbg); 
	sprintf(data, "d: driver");
	g_sputstr(line++,5, data, helpfg, helpbg); 
	sprintf(data, "p: crosspoints");
	g_sputstr(line++,5, data, helpfg, helpbg); 
	sprintf(data, "t: trains");
	g_sputstr(line++,5, data, helpfg, helpbg); 
	sprintf(data, "HJKL:map scroll");
	g_sputstr(line++,5, data, helpfg, helpbg); 
	sprintf(data, "-------------------");
	g_sputstr(line++,5, data, helpfg, helpbg); 
	switch(mode){
		case DRIVERS_MODE:
			sprintf(data, "j: prev loco");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "k: next loco");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "space: reverse");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "h: inc impulse");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "l: dec impulse");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "b: inc brakes       ");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "B: dec brakes");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			break;
		case TRAINS_MODE:	
			sprintf(data, "j: go");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "space: reverse");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "k: move");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "1: link");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "0: unlink");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, ".: change crosspoint");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "a: add wagon");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "A: add loco");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "x: del vehicle");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			break;
		case POINTS_MODE:	
			sprintf(data, "j: prev cross");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, "k: next cross");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			sprintf(data, ".: change direction");
			g_sputstr(line++,5, data, helpfg, helpbg); 
			break;
	}
	delete []data;
}
void g_drawfinder(int row, int col, char aspect, int bg, int fg){
	g_vputc(row, col, aspect,fg,bg); 
}
void g_drawVehicle(int row, int col, char aspect, int bg, int fg, int selected){
	g_vputc(row, col, aspect,selected?bg:fg, selected?fg:bg); 
}
void g_drawCross(int row, int col, char aspect, int fg, int bg, int selected){
	g_vputc(row, col, aspect,selected?bg:fg, selected?fg:bg); 
}
void g_drawRail(int row, int col, char aspect, int fg, int bg){
	g_vputc(row, col, aspect, fg, bg); 
}
void g_clearRail(int row, int col, char aspect, int fg, int bg){
	aspect = ' ';
	g_vputc(row, col, aspect, fg, bg); 
}
void g_alert(const char * texto){
	WINDOW *menu_win;
	int width = 30;
	int height= 10;
	int startx = (80 - width) / 2;
	int starty = (24 - height) / 2;
	menu_win = newwin(height, width, starty, startx);
	box(menu_win, 0, 0);
	wattron(menu_win, A_REVERSE);
	mvwprintw(menu_win, 2, 2, "%s", texto);
	wattroff(menu_win, A_REVERSE);
	wrefresh(menu_win);
	while(wgetch(menu_win)== -1);
	endwin();
}
