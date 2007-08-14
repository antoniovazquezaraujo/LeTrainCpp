/*
  charmap.c
  ASCII plane 
  -------------------------------------
*/
#include "charmap.h"
Charmap::Charmap(){
	int n, m;
	for(n = 0; n<CHAR_MAP_ROWS; n++){
		for(m = 0; m<CHAR_MAP_COLS; m++){
			data[n][m] = '\0';
		}
	}
}

Charmap::~Charmap(){

}
char Charmap::h_char(int row, int col){
	return data[row][col];
}
void Charmap::h_setchar(int row, int col, char c){
	data[row][col] = c;
}
