/*
  railmap.c
  A two dimensional array of rails
  -------------------------------------
*/
#include "letrain.h"


// TODO
// cambiar esta struct por dos funciones diferentes!!!!!!!!!!!!!!!!
struct railandcross{
	Rail * rail;
	Cross * cross;
};

Railmap::Railmap(Charmap * charmap){
	int n, m;
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			rails[n][m] = NULL;
		}
	}
	crosses = new List();
	load(charmap);
}
Railmap::~Railmap(){
	int n, m;
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			delete rails[n][m];
		}
	}
	delete crosses;
}
Rail * Railmap::getRail(int row, int col){
	assert(row < RAIL_MAP_ROWS);
	assert(col < RAIL_MAP_COLS);
	return rails[row][col];
}
void Railmap::setRail(int row, int col, Rail * rail){
	assert(row < RAIL_MAP_ROWS);
	assert(col < RAIL_MAP_COLS);
	rails[row][col] = rail;
}
void Railmap::addCross(Cross * cross){
	crosses->add(cross);
}
List * Railmap::crosslist(){
	return crosses;
}
int Railmap::load(Charmap * charmap){

	struct railandcross rp;
	Rail * r= NULL;
	int row=0, col=0;
	char c;
	for(row = 0; row < RAIL_MAP_ROWS; row++){
		for(col = 0; col < RAIL_MAP_COLS; col++){
			c = charmap->h_char(row,col);
			switch(c){
			case '|':	                         
				r= new Rail( VERTICAL_RAIL, row, col);
				break;                           
			case '-':	                         
				r= new Rail( HORIZONTAL_RAIL, row, col);
				break;                           
			case '\\':	                         
				r= new Rail( DOWN_DIAGONAL_RAIL, row, col);
				break;                           
			case '/':	                         
				r= new Rail( UP_DIAGONAL_RAIL, row, col);
				break;
			case ':':	
				r= newstop( charmap, row, col);
				break;                           
			case '.':	
				r= newcurve( charmap, row, col);
				break;
			case '+':
				rp = newCross(charmap, row, col);
				r= rp.rail;
				addCross(rp.cross);
				break;
			default:
				r= NULL;
			}
			if(r!= NULL){
				setRail(row, col, r);
			}
		}
	}
	connect();
	setcrosses(crosses);
	return row;
}
void Railmap::setcrosses(List * crosses){
	Cross * next;
	Iterator *i = crosses->iterator();
	while(i->hasNext()){
		next = (Cross *)i->next();
		next->nextDir();
	}
	delete i;
}
int Railmap::lookaround (Charmap * charmap, int row, int col, int directions[3][3]){
	int fromRow = 0, toRow = 0;
	int fromCol = 0, toCol = 0;
	int r,c;
	int numFoundDirections = 0; 
	// + in place of point to skip + detection!
	char railChars[3][3]={
		{'\\','|', '/'},
		{'-' ,'+', '-'},
		{'/' ,'|','\\'},
	};

	for(r = 0; r < 3; r++){
		for(c = 0; c < 3; c++){
			directions[r][c] = NO_DIR;
		}
	}
	int posibleDirections[3][3]= {
		{ NW,    N,     NE },
		{ W ,  NO_DIR,  E  },
		{ SW,    S,     SE },
	};

	if(row < 1){
		fromRow = 0;
	}else{
		fromRow = -1;
	}
	if(toRow == CHAR_MAP_ROWS-1){
		toRow = 0;
	}else{
		toRow = 1;
	}
	if(col< 1){
		fromCol= 0;
	}else{
		fromCol = -1;
	}
	if(toCol == CHAR_MAP_COLS-1){
		toCol=0;
	}else{
		toCol=1;
	}
	for(r = fromRow; r <=toRow; r++){
		for(c = fromCol; c <= toCol; c++){
	//		if(1+r == 1 && 1+c == 1) continue; // step over center NO_DIR
			if(charmap->h_char(row+r,col+c) == railChars[1+r][1+c] ){
				directions[1+r][1+c] = posibleDirections[1+r][1+c];
				numFoundDirections++;
			}
		}
	}
	return numFoundDirections;
}
Rail * Railmap::newstop(Charmap * charmap, int row, int col){
	int r = 0, c = 0;
	int foundDirections[3][3] = {
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR}
	};
	int numFoundDirections = 0; 
	int direction;
	Rail * ret = new Rail(STOP_RAIL, row, col);
		
	direction= NO_DIR;
	numFoundDirections = lookaround(charmap, row, col, foundDirections);
	if(numFoundDirections  >  0){
		for(r = 0; r < 3; r++){
			for(c = 0; c < 3; c++){
				if(foundDirections[r][c] != NO_DIR){
					direction= foundDirections[r][c];
					break;
				}
			}
		}
	}

	if(direction != NO_DIR){
		ret->setDir(direction, direction);
		ret->setDir(invert(direction), direction);
	}
	return ret; 
}
Rail * Railmap::newcurve(Charmap * charmap, int row, int col){
	int r = 0, c = 0;
	int foundDirections[3][3] = {
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR}
	};
	int numFoundDirections = 0; 
	int directions[2]= {NO_DIR, NO_DIR};

	Rail * ret = new Rail(CURVE_RAIL, row, col);
	numFoundDirections= lookaround(charmap, row, col, foundDirections);	
	assert(numFoundDirections == 2);
		
	int n = 0;
	for(r = 0; r < 3; r++){
		for(c = 0; c < 3; c++){
			if(foundDirections[r][c] != NO_DIR){
				directions[n++] = foundDirections[r][c];
			}
		}
	}

	ret->setDir(directions[0], directions[1]);
	ret->setDir(directions[1], directions[0]);
	return ret; 
}
int Railmap::getCrossDir(int directions[][3]){
	int posibleDirections[3][3]= {
		{ NO_DIR,    N,    NO_DIR },
		{ W     ,  NO_DIR,      E },
		{ NO_DIR,    S,    NO_DIR },
	};
	int r,c = 0;
	int foundCol = 0;
	int foundRow = 0;
	int uniDirectionRow = -1;
	int uniDirectionCol= -1;
	int directionsByRow = 0;
	int directionsByCol= 0;
	int ret = NO_DIR; 

	// search a row with only one direction
	foundCol = 0;
	directionsByRow = 0;
	for(r = 0; r < 3; r++){  
		directionsByRow=0;
		for(c = 0; c < 3; c++){
			if(directions[r][c] != NO_DIR){
				directionsByRow++;
				foundCol = c;
			}
		}
		// if this is the row with the input direction...
		if((directionsByRow == 1) && (foundCol == 1)){
			uniDirectionRow = r;
		}
	}
	if(uniDirectionRow != -1){
		ret = posibleDirections[uniDirectionRow][1];
	}else{

		foundRow = 0;
		directionsByCol= 0;
		// search a col with only one direction
		for(c = 0; c < 3; c++){
			directionsByCol = 0;
			for(r = 0; r < 3; r++){
				if(directions[r][c] != NO_DIR){
					directionsByCol++;
					foundRow = r;
				}
			}
			// if this is the col with the input direction...
			if((directionsByCol== 1) && (foundRow == 1)){
				uniDirectionCol=c;
			}
		}
		if(uniDirectionCol != -1){
			ret = posibleDirections[1][uniDirectionCol];
		}
	}
	return ret;
}
int Railmap::crossouts (int directions[3][3], int outs[3], int keyDir){
	int col, row = 0;
	int n= 0;
	switch(keyDir){
	case N:
		for(col = 0; col < 3; col++){
			if(directions[2][col] != NO_DIR){
				outs[n++] = directions[2][col];
			}
		}
		break;	
	case S:
		for(col = 0; col < 3; col++){
			if(directions[0][col] != NO_DIR){
				outs[n++] = directions[0][col];
			}
		}
		break;	
	case W:
		for(row = 0; row < 3; row++){
			if(directions[row][2] != NO_DIR){
				outs[n++] = directions[row][2];
			}
		}
		break;	
	case E:
		for(row = 0; row < 3; row++){
			if(directions[row][0] != NO_DIR){
				outs[n++] = directions[row][0];
			}
		}
		break;	
	}
	return n;

}
struct railandcross Railmap::newCross(Charmap * charmap, int row, int col){
	int foundDirections[3][3] = {
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR}
	};
	int numFoundDirections = 0; 
	int outs[3] = {NO_DIR, NO_DIR, NO_DIR};
	int n;
	Rail * ret = new Rail(CROSS_RAIL, row, col);

	numFoundDirections = lookaround(charmap, row, col, foundDirections);
	int crossKeyDir = getCrossDir(foundDirections);
	int numDirs = crossouts(foundDirections, outs, crossKeyDir);

	if(crossKeyDir != NO_DIR){
		ret->setDir(crossKeyDir, outs[0]); //simply the first
	}
	for(n = 0; n<numDirs; n++){
		ret->setDir(outs[n], crossKeyDir);
	}
	Cross * c= new Cross(ret, crossKeyDir, outs);
	struct railandcross rp;
	rp.rail = ret;
	rp.cross = c;

	return rp; 
}
void Railmap::connect(){
	int dir=NO_DIR;
	Rail * rail = NULL; 
	int row=0, col=0;
	Point p(0,0);
	for(row = 0; row < RAIL_MAP_ROWS; row++){
		for(col = 0; col < RAIL_MAP_COLS; col++){
			rail = getRail(row, col);
			if(rail == NULL){
				continue;
			}
			for(dir = 0; dir < 8; dir++){
				int foundDir = rail->getDir(dir);
				if(foundDir == NO_DIR){
					continue;
				}
				p.row = row;
				p.col = col;
				p.move(dir);
				assert(p.row < RAIL_MAP_ROWS);
				assert(p.col < RAIL_MAP_COLS);
				Rail * linked = getRail(p.row, p.col );
				if(linked != NULL){
					rail->setlink(dir, linked);
				}
			}
		}
	}
}

