/*
  railmap.c
  A two dimensional array of rails
  -------------------------------------
*/
#include "letrain.h"


// TODO
// cambiar esta struct por dos funciones diferentes!!!!!!!!!!!!!!!!
struct railandcross{
	Rail rail;
	Cross cross;
};

static int lookaround (Charmap charmap, int row, int col, int directions[3][3]);
static Rail newcurve(Charmap charmap, int row, int col);
static struct railandcross newcross(Charmap charmap, int row, int col);
static Rail newstop(Charmap charmap, int row, int col);
static int crossouts (int directions[3][3], int outs[3], int keyDir);
static void connect(Railmap railmap);
static void setcrosses(List crosses);

/*
static void setcharmapdata(Charmap charmap, int row, int col, char c);
*/

Railmap m_new(Charmap charmap){
	int n, m;
	Railmap railmap = malloc(sizeof(struct railmap));
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			railmap->rails[n][m] = NULL;
		}
	}
	railmap->crosses = l_new();
	m_load(railmap, charmap);
	return railmap;
}
/*
Railmap m_new(){
	int n, m;
	Railmap railmap = malloc(sizeof(struct railmap));
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			railmap->rails[n][m] = NULL;
		}
	}
	railmap->crosses = l_new();
	return railmap;
}
*/
void m_free(Railmap railmap){
	int n, m;
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			r_free(railmap->rails[n][m]);
		}
	}
	l_free(railmap->crosses);
	free(railmap);
}
Rail m_rail(Railmap railmap, int row, int col){
	assert(row < RAIL_MAP_ROWS);
	assert(col < RAIL_MAP_COLS);
	return railmap->rails[row][col];
}
void m_setrail(Railmap railmap, int row, int col, Rail rail){
	assert(row < RAIL_MAP_ROWS);
	assert(col < RAIL_MAP_COLS);
	railmap->rails[row][col] = rail;
}
void m_addcross(Railmap railmap, Cross cross){
	l_add(railmap->crosses, cross);
}
List m_crosslist(Railmap railmap){
	return railmap->crosses;
}
int m_load(Railmap railmap, Charmap charmap){

	struct railandcross rp;
	Rail rail = NULL;
	int row=0, col=0;
	char c;
	for(row = 0; row < RAIL_MAP_ROWS; row++){
		for(col = 0; col < RAIL_MAP_COLS; col++){
			c = h_char(charmap, row,col);
			switch(c){
			case '|':	                         
				rail = r_new( VERTICAL_RAIL, row, col);
				break;                           
			case '-':	                         
				rail = r_new ( HORIZONTAL_RAIL, row, col);
				break;                           
			case '\\':	                         
				rail = r_new ( DOWN_DIAGONAL_RAIL, row, col);
				break;                           
			case '/':	                         
				rail = r_new ( UP_DIAGONAL_RAIL, row, col);
				break;
			case ':':	
				rail = newstop( charmap, row, col);
				break;                           
			case '.':	
				rail = newcurve( charmap, row, col);
				break;
			case '+':
				rp = newcross(charmap, row, col);
				rail = rp.rail;
				lg("m_load",1,"cross: in: %d, outs: %d %d %d", rp.cross->in, rp.cross->outs[0], rp.cross->outs[1], rp.cross->outs[2]);
				m_addcross(railmap, rp.cross);
				break;
			default:
				rail = NULL;
			}
			if(rail != NULL){
				m_setrail(railmap, row, col, rail);
			}
		}
	}
	connect(railmap);
	setcrosses(railmap->crosses);
	return row;
}
/*
int m_load(Railmap railmap, char * filename){
	Charmap charmap = newcharmap();
	loadcharmap(charmap, filename);

	struct railandcross rp;
	Rail rail = NULL;
	int row=0, col=0;
	char c;
	for(row = 0; row < RAIL_MAP_ROWS; row++){
		for(col = 0; col < RAIL_MAP_COLS; col++){
			c = charmapdata(charmap, row, col);
			switch(c){
			case '|':	                         
				rail = r_new( VERTICAL_RAIL, row, col);
				break;                           
			case '-':	                         
				rail = r_new ( HORIZONTAL_RAIL, row, col);
				break;                           
			case '\\':	                         
				rail = r_new ( DOWN_DIAGONAL_RAIL, row, col);
				break;                           
			case '/':	                         
				rail = r_new ( UP_DIAGONAL_RAIL, row, col);
				break;
			case ':':	
				rail = newstop( charmap, row, col);
				break;                           
			case '.':	
				rail = newcurve( charmap, row, col);
				break;
			case '+':
				rp = newcross(charmap, row, col);
				rail = rp.rail;
				m_addcross(railmap, rp.cross);
				break;
			default:
				rail = NULL;
			}
			if(rail != NULL){
				m_setrail(railmap, row, col, rail);
			}
		}
	}
	connect(railmap);
	setcrosses(railmap->crosses);
	freecharmap(charmap);
	return row;
}
*/
static void setcrosses(List crosses){
	Cross next;
	Iterator i = i_new(crosses);
	while(i_hasnext(i)){
		next = (Cross)i_next(i);
		x_nextdir(next);
	}
	i_free(i);
}
static int lookaround (Charmap charmap, int row, int col, int directions[3][3]){
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
			if(h_char(charmap,row+r,col+c) == railChars[1+r][1+c] ){
				directions[1+r][1+c] = posibleDirections[1+r][1+c];
				numFoundDirections++;
			}
		}
	}
	return numFoundDirections;
}
static Rail newstop(Charmap charmap, int row, int col){
	int r = 0, c = 0;
	int foundDirections[3][3] = {
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR}
	};
	int numFoundDirections = 0; 
	int direction;
	Rail ret = r_new(STOP_RAIL, row, col);
		
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
		r_setdir(ret, direction, NO_DIR);
		r_setdir(ret, d_invert(direction), direction);
	}
	return ret; 
}
static Rail newcurve(Charmap charmap, int row, int col){
	int r = 0, c = 0;
	int foundDirections[3][3] = {
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR}
	};
	int numFoundDirections = 0; 
	int directions[2]= {NO_DIR, NO_DIR};

	Rail ret = r_new(CURVE_RAIL, row, col);
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

	r_setdir(ret, directions[0], directions[1]);
	r_setdir(ret, directions[1], directions[0]);
	return ret; 
}
int crossdir(int directions[3][3]){
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
static int crossouts (int directions[3][3], int outs[3], int keyDir){
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
static struct railandcross newcross(Charmap charmap, int row, int col){
	int foundDirections[3][3] = {
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR},
		{NO_DIR, NO_DIR, NO_DIR}
	};
	int numFoundDirections = 0; 
	int outs[3] = {NO_DIR, NO_DIR, NO_DIR};
	int n;
	Rail ret = r_new(CROSS_RAIL, row, col);

	numFoundDirections = lookaround(charmap, row, col, foundDirections);
	int crossKeyDir = crossdir(foundDirections);
	int numDirs = crossouts(foundDirections, outs, crossKeyDir);

	if(crossKeyDir != NO_DIR){
		r_setdir(ret, crossKeyDir, outs[0]); //simply the first
	}
	for(n = 0; n<numDirs; n++){
		r_setdir(ret, outs[n], crossKeyDir);
	}
	Cross cross = x_new(ret, crossKeyDir, outs);
	struct railandcross rp;
	rp.rail = ret;
	rp.cross = cross;

	return rp; 
}
static void connect(Railmap railmap){
	int dir=NO_DIR;
	Rail rail = NULL; 
	int row=0, col=0;
	struct point p = {0,0};
	for(row = 0; row < RAIL_MAP_ROWS; row++){
		for(col = 0; col < RAIL_MAP_COLS; col++){
			rail = m_rail(railmap, row, col);
			if(rail == NULL){
				continue;
			}
			for(dir = 0; dir < 8; dir++){
				int foundDir = r_dir(rail, dir);
				if(foundDir == NO_DIR){
					continue;
				}
				p.row = row;
				p.col = col;
				p_move(&p, dir);
				assert(p.row < RAIL_MAP_ROWS);
				assert(p.col < RAIL_MAP_COLS);
				Rail linked = m_rail(railmap, p.row, p.col );
				if(linked != NULL){
					r_setlink(rail, dir, linked);
				}
			}
		}
	}
}

/*
void setcharmapdata(Charmap charmap, int row, int col, char c){
	charmap->data[row][col] = c;
}
*/
