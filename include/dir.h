#ifndef DIR_H
#define DIR_H
enum{
	NO_DIR =-1,
	E   =0,
	NE  =1,
	N   =2,
	NW  =3,
	W   =4,
	SW  =5,
	S   =6,
	SE  =7
};
int invert(int direction);
void dirtostr(int dir, char* dest);
int strToDir(char *c);
#endif
