#ifndef CHARMAP_H
#define CHARMAP_H

#define CHAR_MAP_ROWS 500
#define CHAR_MAP_COLS 500 
class Charmap {
public:
	Charmap();
	~Charmap();
	char h_char(int row, int col); 
	void h_setchar(int row, int col, char c); 
	char data[CHAR_MAP_ROWS][CHAR_MAP_COLS];
};
#endif
