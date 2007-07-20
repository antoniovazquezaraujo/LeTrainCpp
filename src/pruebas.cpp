class Dir{
public:
	enum dir {NO_DIR=-1, E, NE, N, NW, W, SW, S, SE};
	dir value;      
	Dir(dir n =NO_DIR);
	dir invert(dir d);
};
Dir::Dir(Dir::dir n): value(n){

}
Dir::dir Dir::invert(Dir::dir d){
	return Dir::SE;
}
int main2(){
	Dir d, e(Dir::NE);
	Dir::dir x = e.invert(Dir::N);
}
