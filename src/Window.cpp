#include "Basic.h"
#include "Window.h"
MAKE_LOGGER(Window);

Window::Window(int row, int col, int width, int height, bool withBorder)
	: row(row), col(col), width(width), height(height), withBorder(withBorder) {
	if(withBorder){
		border = newwin(height, width, row, col);
		box(border, 0,0);
		this->height-=2;
		this->width-=2;
		this->row++;
		this->col++;
		window = newwin(this->height, this->width, this->row, this->col);
	}else{
		window = newwin(height, width, row, col);
		border=0;
	}	
	//panel = new_panel(window);
	reversed=0;
	bold = 0;
	underlined = 0;
	fgColor = 1;
	bgColor = 0;
	wstandout(window);
	vShift=0;
	hShift=0;
	show(true);
}
void Window::show(bool b){
	if(b){
		//show_panel(panel);
	}else{
		//hide_panel(panel);
	}
	wrefresh(window);
	//update_panels();
	doupdate();
}
void Window::setup(){
	initscr();    
	clear();
	keypad(stdscr, true);
	nonl();             
	nocbreak();
	noecho();
	curs_set(false);     
    if (has_colors()) {
		start_color();
		for(int n = 0; n<8; n++){
			for(int m=0;m<8;m++){
				init_pair(n*8+m, n, m);
			}
		}
	}
}
void Window::cleanup(){
    endwin();
}
int Window::getScreenCols(){
	int screenCols, x;
	getmaxyx(stdscr, x, screenCols);
	return screenCols;
}
int Window::getScreenRows(){
	int screenRows, x;
	getmaxyx(stdscr, screenRows, x);
	return screenRows;
}
int Window::getKey(){
	if(halfdelay(1) != ERR){
		return getch();  
	}else{
		return -1;
	}
}
int Window::waitKey(){
	return getch();  
}
Window::~Window(){
	wattron(window, COLOR_PAIR(fgColor*8 + bgColor));
	wborder(window,' ',' ',' ',' ',' ',' ',' ',' '); 
	wattroff(window, COLOR_PAIR(fgColor*8 + bgColor));
	delwin(window);
}
void Window::repaint(){
	if(border) wrefresh(border);
	wrefresh(window);
}
int Window::getWidth(){
	return width;
}
int Window::getHeight(){
	return height;
}	
void Window::shift(int mode, int size){
	if(mode == HORIZONTAL_SHIFT){
		hShift = size-getWidth()/2;
	}else{
		vShift = size-getHeight()/2;
	}
}
void Window::setReversed(bool r){
	if(r){
		this->reversed = A_REVERSE;
	}else{
		this->reversed = 0;
	}
	wattrset(window, COLOR_PAIR(fgColor*8 + bgColor)|bold|reversed|underlined);
}
void Window::setUnderlined(bool r){
	if(r){
		this->underlined= A_UNDERLINE;
	}else{
		this->underlined= 0;
	}
	wattrset(window, COLOR_PAIR(fgColor*8 + bgColor)|bold|reversed|underlined);
}
void Window::erase(void){
	werase(window);
}
void Window::setFg(int color){
	if(color >=8){
		fgColor = color -8;	
		bold = A_BOLD;	
	}else{
		bold = 0;
		fgColor = color;
	}
	wattrset(window, COLOR_PAIR(fgColor*8 + bgColor)|bold|reversed|underlined);
}
void Window::setBg(int color){
	if(color >=8){
		bgColor = color -8;	
		bold = A_BOLD;	
	}else{
		bgColor = color;
		bold = 0;	
	}
	wattrset(window, COLOR_PAIR(fgColor*8 + bgColor)|bold|reversed|underlined);
}
void Window::setTitle(const char * str){
	int len = strlen(str);
	int pos = (width - len) / 2;
	wattron(border, COLOR_PAIR(bgColor*8 + fgColor));
	mvwaddstr(border, 0, pos,str);
	wattroff(border, COLOR_PAIR(bgColor*8 + fgColor));
}
void Window::putC(int row, int col, char c){
	mvwaddch(window, row-vShift,col-hShift,c);
}
void Window::putStr(int row, int col, const char *str){
	mvwaddstr(window, row-vShift,col-hShift,str);
}

