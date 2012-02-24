#include "Basic.h"
#include "TerminalWindow.h"
MAKE_LOGGER(TerminalWindow);

TerminalWindow::TerminalWindow(int row, int col, int width, int height, bool withBorder)
	:Window(row, col, width, height, withBorder){
		scrollok(window, true);
		idlok(window, true);
		setFg(7);
		setBg(0);
}
void TerminalWindow::show(bool b){
	if(b){
		cbreak();
		echo();
		nl();             
		curs_set(2);     
	}else{
		nocbreak();
		noecho();
		nonl();             
		curs_set(0);     
	}
	Window::show(b);
}

TerminalWindow & TerminalWindow::operator << (const char s[]){
	wprintw(window, "%s", s);
	return *this;
}
TerminalWindow & TerminalWindow::operator << (const string & s){
	wprintw(window, "%s", s.c_str());
	return *this;
}
TerminalWindow & TerminalWindow::operator << (int i){
	wprintw(window, "%d", i);
	return *this;
}
TerminalWindow & TerminalWindow::operator << (char c){
	wprintw(window, "%c", c);
	return *this;
}
TerminalWindow & TerminalWindow::operator >> (string & s){
	char *tmp;
	int l = s.length();
	if(l > 0){
		tmp = new char[200];
	}else{
		tmp = new char[200];
	}
	wgetstr(window, tmp);
	s = tmp;
	delete[] tmp;
	return *this;
}
TerminalWindow & TerminalWindow::operator >> (int & i){
	char buff[6];
	wgetnstr(window, buff, 6);
	i = atoi(buff);
	LOG_DEBUG(log," Leyendo una cadena con enteros: " << i);
	return *this;
}
TerminalWindow & TerminalWindow::operator >> (char & c){
	c = wgetch(window);
	return *this;
}
