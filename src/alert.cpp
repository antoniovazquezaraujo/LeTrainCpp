#include <ncurses.h> 

#define WIDTH 30
#define HEIGHT 10

void alert(char * texto);
int main3() {
	initscr();
	alert("hola hola caracola");
}

void alert(char * texto){
	WINDOW *menu_win;
	int startx = (80 - WIDTH) / 2;
	int starty = (24 - HEIGHT) / 2;
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	box(menu_win, 0, 0);
	wattron(menu_win, A_REVERSE);
	mvwprintw(menu_win, 2, 2, "%s", texto);
	wattroff(menu_win, A_REVERSE);
	wrefresh(menu_win);
	wgetch(menu_win);
	endwin();
}


