/*
  graphics.h
  The curses interface 
  -------------------------------------
*/
#ifndef WINDOW_H
#define WINDOW_H
#include "Basic.h"
#include <curses.h>
#include <panel.h>
class Window{
public:
	Window(int row, int col, int width, int height, bool withBorder = true);
	~Window();
	static void setup();
	static void resize();
	static void cleanup();
	static int getKey();
	static int waitKey();
	void setFg(int color);
	void setBg(int color);
	void setReversed(bool);
	void setUnderlined(bool);
	void putC(int row, int col, int c);
	void putStr(int row, int col, const char *str);
	void setTitle(const char * str);
	void repaint(void);
	void erase(void);
	static int getScreenCols();
	static int getScreenRows();
	void shift(int mode, int size);
	enum {VERTICAL_SHIFT, HORIZONTAL_SHIFT};
	int getWidth();
	int getHeight(); 
	virtual void show(bool);
protected:
	WINDOW * window;
	WINDOW * border;
	PANEL * panel;
	int row, col, width, height;
	bool withBorder;
	int fgColor, bgColor;
	int reversed, bold, underlined;
	int hShift, vShift;
	
};
#endif
