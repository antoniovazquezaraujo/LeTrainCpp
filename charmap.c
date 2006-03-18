/*
  charmap.c
  ASCII plane 
  -------------------------------------
*/
#include "letrain.h"
Charmap h_new(){
	int n, m;
	Charmap charmap = malloc(sizeof(struct charmap));
	for(n = 0; n<CHAR_MAP_ROWS; n++){
		for(m = 0; m<CHAR_MAP_COLS; m++){
			charmap->data[n][m] = '\0';
		}
	}
	return charmap;
}
char h_char(Charmap map, int row, int col){
	return map->data[row][col];
}
void h_setchar(Charmap map, int row, int col, char c){
	map->data[row][col] = c;
}
