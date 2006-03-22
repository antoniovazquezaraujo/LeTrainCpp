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
void    g_putstr(int row, int col, char *str, int fgColor, int bgColor);
void	g_repaint(void);
void    g_vputc(int row, int col, char c, int fgColor, int bgColor);
void    g_sputstr(int row, int col, char *str, int fgColor, int bgColor);
void    g_vset(int row, int col, int rows, int cols);
void    g_sset(int row, int col, int rows, int cols);
void    g_drawvehiclestatus(int id, int row, int col, int dir, int imp, int power, int brakes, int link0id, int link1id, char aspect);
void    g_drawhelp();
void    g_drawfinder(int row, int col, char aspect, int bg, int fg);
void    g_drawvehicle(int row, int col, char aspect, int bg, int fg, int selected);
void    g_drawcross(int row, int col, char aspect, int fg, int bg, int selected);
void    g_drawrail(int row, int col, char aspect, int bg, int fg);
void    g_clearrail(int row, int col, char aspect, int bg, int fg);
