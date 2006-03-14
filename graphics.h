/*
  graphics.h
  The curses interface 
  -------------------------------------
*/
#include <ncurses.h>
void	g_init(void);
void	g_reset(void);
int 	g_getkey(void);
void	g_putc(int row, int col, char c, int fg_color, int bg_color);
void	g_repaint(void);
