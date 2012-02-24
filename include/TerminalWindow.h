/*
  graphics.h
  The curses interface 
  -------------------------------------
*/
#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H
#include "Basic.h"
#include "Window.h"
class TerminalWindow :public Window{
public:
	TerminalWindow(int row, int col, int width, int height, bool withBorder = true);
	~TerminalWindow();
	void show(bool);
	TerminalWindow & operator << (const char s[]);
	TerminalWindow & operator << (const string &s);
	TerminalWindow & operator << (int i);
	TerminalWindow & operator >> (string & s);
	TerminalWindow & operator >> (int & i);
	TerminalWindow & operator >> (char & c);
	TerminalWindow & operator << (char c);
private:
	static Logger log;
};
#endif
