#include <cstdlib>
#include <cstring>
#include <cpptest.h>
#include "Basic.h"
#include "Dir.h"
#include "DirEnv.h"
#include "Window.h"
#include "Commander.h"
#include "Sim.h"
#include "Rail.h"
#include "Aspect.h"
#include "Game.h"
#include "Finder.h"
#include "ForkDirSelector.h"
#include "ForkRail.h"
#include "Range.h"
#include "Locomotive.h"
#include "Train.h"

#define BEGIN_TEST if(true){ setup();
#define END_TEST tear_down();} 
class TestSuite1 : public Test::Suite {
private:
	DirEnv *env, *env1, *env2;
	Dir * d;
	static Logger log;
public:
	TestSuite1() {
		TEST_ADD(TestSuite1::test)
	}
protected:
	virtual void setup(){
		d = new Dir();
		env1 = new DirEnv;
		env2 = new DirEnv;
		env = new DirEnv();
	}
	virtual void tear_down(){
		delete d;
		d=0;
		delete env;
		env = 0;
		delete env1;
		env1=0;
		delete env2;
		env2=0;
	}

	void test(){


/******
  Test Un Finder se pone sobre una recta y se mueve adelante y atras y se comprueba
  que toma la posicion correcta
 ******/

BEGIN_TEST
	env1->addPath(Dir::E, Dir::W);
	env2->addPath(Dir::W, Dir::E);
	Rail * r1 = env1->makeNewRail();
	r1->setPos(Point(10,10));
	Rail * r2 = env2->makeNewRail();
	r2->setPos(Point(10,9));
	r1->linkRailAt(Dir::W, r2);
	r2->linkRailAt(Dir::E, r1);
	Finder f;
	f.setDir(Dir::W);
	f.gotoRail(r1);
	TEST_ASSERT(f.getPos().row == 10);
	TEST_ASSERT(f.getPos().col == 10);
	f.forward();
	TEST_ASSERT(f.getPos().row == 10);
	TEST_ASSERT(f.getPos().col == 9);
	f.reverse();
	f.forward();
	TEST_ASSERT(f.getPos().row == 10);
	TEST_ASSERT(f.getPos().col == 10);

END_TEST

/******
  Test A un DirEnv se le agregan las ocho rectas por separado y se crea un Rail con el.
  Luego se coloca un Finder en el Rail y se comprueba que dando la direccion de entrada
  obtiene la direccion de salida
 ******/
BEGIN_TEST
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n);
		DirEnv env;
		env.addPath(d,-d);
		Rail * r = env.makeNewRail();
		Finder f;
		f.setDir(d);
		f.gotoRail(r);
		TEST_ASSERT(f.getDir().getValue() == r->getPath(-d).getValue());
		delete r;
	}
END_TEST
/******
  Test Un ForkDirSelector selecciona su direccion correctamente
******/

BEGIN_TEST
	DirEnv * env = new DirEnv;
	env->addPath(Dir::E, Dir::NW);
	env->addPath(Dir::E, Dir::SW);
	ForkDirSelector f(env);
	f.selectDir(Dir::NW);
	TEST_ASSERT(f.getSelectedDir() == Dir::NW);
	f.selectDir(Dir::SW);
	TEST_ASSERT(f.getSelectedDir() == Dir::SW);
END_TEST
/******
  Test  Dos Rails son enlazados correctamente
******/

BEGIN_TEST
	DirEnv env1, env2;
	env1.addPath(Dir::E, Dir::W);
	env2.addPath(Dir::W, Dir::E);
	Rail * r1 = env1.makeNewRail();
	Rail * r2 = env2.makeNewRail();
	r1->linkRailAt(Dir::W, r2);
	TEST_ASSERT(r1->getLinkedRailAt(Dir::W) == r2);
	r2->linkRailAt(Dir::E, r1);
	TEST_ASSERT(r2->getLinkedRailAt(Dir::E) == r1);
END_TEST

/******
  Test  Una Dir invertida con - es igual a sumarle el angulo medio
******/

BEGIN_TEST
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n);
		Dir e = -d;
		TEST_ASSERT(e== d+ Dir::MIDDLE_ANGLE);
	}

END_TEST

/******
  Test A un DirEnv con 8 rectas distintas le pedimos que cree un Rail y las
  direcciones de entrada y salida son las correctas
******/
BEGIN_TEST
	Dir real, esperada;
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n); 
		env->addPath(d, d + Dir::MIDDLE_ANGLE);
		real = env->getPath(d);
		esperada = d+Dir::MIDDLE_ANGLE;
		TEST_ASSERT(real.getValue() == esperada.getValue());
		Rail * r = env->makeNewRail();
		TEST_ASSERT(r->getPath(d + Dir::MIDDLE_ANGLE) == d);
	}
END_TEST

/******
  Test  A un DirEnv le agregamos primero una recta y luego el resto
   Comprobamos que el rail creado tiene aspecto de cruce
******/
BEGIN_TEST	
	env->addPath(Dir::E, Dir::W);
	for(int n(Dir::NE) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n); 
		env->addPath(d, d + Dir::MIDDLE_ANGLE);
		TEST_ASSERT(env->getPath(d) == d + Dir::MIDDLE_ANGLE);
		Rail * r = env->makeNewRail();
		int c = r->getAspect()->getAspectChar();
		TEST_ASSERT(c == '+');
		//delete r;
	}
END_TEST

/******
  Test  A un DirEnv le agregamos curvas MIN y comprobamos que tienen aspecto de curva
******/
BEGIN_TEST
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n); 
		DirEnv * env = new DirEnv();
		env->addPath(d, d+Dir::MIN_CURVE_ANGLE);
		TEST_ASSERT(env->getPath(d) == d+Dir::MIN_CURVE_ANGLE);
		Rail * r = env->makeNewRail();
		Aspect * aspect = r->getAspect();
		int c = aspect->getAspectChar();
		TEST_ASSERT(c == '.');
		delete r;
		delete env;
	}
END_TEST

/******
  Test  A un DirEnv le agregamos curvas MAX y comprobamos que tienen aspecto de curva
******/
BEGIN_TEST	
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n); 
		DirEnv * env = new DirEnv();
		env->addPath(d, d+Dir::MAX_CURVE_ANGLE);
		TEST_ASSERT(env->getPath(d) == d+Dir::MAX_CURVE_ANGLE);
		Rail * r = env->makeNewRail();
		int c = r->getAspect()->getAspectChar();
		TEST_ASSERT(c == '.');
		delete env;
	}
END_TEST

/******
  Test  A un DirEnv le agregamos forks de MIN y MAX y comprobamos 
   que el rail creado selecciona bien las dos direcciones
******/
BEGIN_TEST
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir d(n); 
		DirEnv * env1 = new DirEnv();
		env1->addPath(d, d+Dir::MIN_CURVE_ANGLE);
		env1->addPath(d, d+Dir::MAX_CURVE_ANGLE);
		TEST_ASSERT(env1->getPath(d+Dir::MAX_CURVE_ANGLE) == d);
		TEST_ASSERT(env1->getPath(d+Dir::MIN_CURVE_ANGLE) == d);
		ForkRail * r = (ForkRail*)env1->makeNewRail();
		LOG_DEBUG(log,"ForkRail: " <<  *r) 
			r->selectDir(d+Dir::MIN_CURVE_ANGLE);
		TEST_ASSERT(r->getSelectedDir() == d+Dir::MIN_CURVE_ANGLE);
		r->selectDir(d+Dir::MAX_CURVE_ANGLE);
		TEST_ASSERT(r->getSelectedDir() == d+Dir::MAX_CURVE_ANGLE);
		delete env1;
	}
END_TEST

/******
  Test  A un DirEnv le agregamos forks de MIN y MAX y Dir::MIDDLE_ANGLE y comprobamos 
	   que el rail creado selecciona bien las tres direcciones
******/
BEGIN_TEST
	DirEnv * env1;
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		env1 = new DirEnv;
		Dir d(n); 
		env1->addPath(d, d+Dir::MIN_CURVE_ANGLE);
		env1->addPath(d, d+Dir::MIDDLE_ANGLE);
		env1->addPath(d, d+Dir::MAX_CURVE_ANGLE);
		TEST_ASSERT(env1->getPath(d+Dir::MAX_CURVE_ANGLE) == d);
		TEST_ASSERT(env1->getPath(d+Dir::MIDDLE_ANGLE) == d);
		TEST_ASSERT(env1->getPath(d+Dir::MIN_CURVE_ANGLE) == d);
		ForkRail * r = (ForkRail*)env1->makeNewRail();
		r->selectDir(d+Dir::MIN_CURVE_ANGLE);
		LOG_DEBUG(log,"d+Dir::MIN_CURVE_ANGLE:" << *r);
		TEST_ASSERT(r->getSelectedDir() == d+Dir::MIN_CURVE_ANGLE);
		r->selectDir(d+Dir::MIDDLE_ANGLE);
		LOG_DEBUG(log,"d+Dir::MIDDLE_ANGLE:   " << *r);
		TEST_ASSERT(r->getSelectedDir() == d+Dir::MIDDLE_ANGLE);
		r->selectDir(d+Dir::MAX_CURVE_ANGLE);
		LOG_DEBUG(log,"d+Dir::MAX_CURVE_ANGLE:" << *r);
		TEST_ASSERT(r->getSelectedDir() == d+Dir::MAX_CURVE_ANGLE);
		delete r;
		delete env1;
	}
END_TEST

/******
  Test A una Dir le ponemos las 8 direcciones y  Dir::NO_DIR y comprobamos que las devuelve 
******/
BEGIN_TEST 
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		d->setValue(n);
		TEST_ASSERT(d->getValue() == n);
	}
	d->setValue(Dir::NO_DIR);
	TEST_ASSERT(d->getValue() == Dir::NO_DIR);
END_TEST

/******
  Test En una Dir comprobamos los -- y los ++ pre y post 
******/
BEGIN_TEST 
	d->setValue(Dir::E);
	for(int n=Dir::E; n<Dir::NUM_DIRS ;n++){
		TEST_ASSERT(d->getValue() == n);
		(*d)++;
	}
	d->setValue(Dir::E);
	for(int n=Dir::E; n<Dir::NUM_DIRS ;n++){
		TEST_ASSERT(d->getValue() == n);
		++(*d);
	}
	d->setValue(Dir::SE);
	for(int n=Dir::SE; n>= 0;n--){
		TEST_ASSERT(d->getValue() == n);
		(*d)--;
	}
	d->setValue(Dir::SE);
	for(int n=Dir::SE; n>= 0;n--){
		TEST_ASSERT(d->getValue() == n);
		--(*d);
	}
END_TEST

/******
  Test Una Dir es menor y una Dir es igual a otra 
******/
BEGIN_TEST 
	Dir x, y;
	for(int n=Dir::E; n<Dir::NUM_DIRS ;n++){
		TEST_ASSERT(x == y);
		x++;
		y++;
	}
	x.setValue(Dir::E);
	y.setValue(Dir::NE);
	for(int n=Dir::E; n<Dir::NUM_DIRS-1 ;n++){
		TEST_ASSERT(x < y);
		x++;
		y++;
	}
END_TEST

/******
  Test La inversion de Dirs es correcta 
******/

BEGIN_TEST
	Dir a, b;
	for(int n=0; n<=4;n++){
		TEST_ASSERT(a-b == n);
		a++;
	}
	for(int n=3; n>0;n--){
		TEST_ASSERT(a-b == n*-1);
		a++;
	}
END_TEST

/******
  Test Una Dir forma curva con otra 
******/

BEGIN_TEST
	Dir a(Dir::E), b(Dir::W);
	for(int n=0; n<Dir::NUM_DIRS;n++){
		TEST_ASSERT(a.isStraight(b));
		a++;
		b++;
	}
	a.setValue(Dir::W);
	b.setValue(Dir::E);
	b++;
	TEST_ASSERT(a.isCurve(b));
	b--;
	b--;
	TEST_ASSERT(a.isCurve(b));
	a.setValue(Dir::E);
	b.setValue(Dir::W);
	b++;
	TEST_ASSERT(a.isCurve(b));
	b--;
	b--;
	TEST_ASSERT(a.isCurve(b));
	a.setValue(Dir::NE);
	b.setValue(Dir::SW);
	b++;
	TEST_ASSERT(a.isCurve(b));
	b--;
	b--;
	TEST_ASSERT(a.isCurve(b));
END_TEST
/*
   Test para probar las sentencias.
   */
BEGIN_TEST
	extern bool parse(string s, Game * game);
	/* 
	   Al usar Window::setup() se cambia el modo de debug y no 
	   funcionan los TEST_ASSERT. Los cambio por simples assert
	   */

	Window::setup();
	Game * g = new Game();
	vector<Locomotive*>& locos = g->sim.getLocomotives();
	vector<Wagon*>     & wagons= g->sim.getWagons();
	parse("set bulldozer row(0) col(0) dir(0) mode(painting)",g);
	parse("move bulldozer   100", g);
	parse("add locomotive 1 row (0) col (50) speed(10)", g);      
	parse("add wagon 1 row(0) col(51)", g);                       
	assert(locos.size() == 1);
	assert(wagons.size() == 1);
	parse("add wagon 2 row(0) col(52)", g);                       
	assert(wagons.size() == 2);
	g->sim.moveTrains();
	//g->sim.moveWagons();
	g->sim.checkSensors();
	//g->paint();
	Window::cleanup();
END_TEST

BEGIN_TEST
	env1->addPath(Dir::E, Dir::W);
	env2->addPath(Dir::W, Dir::E);
	Rail * r1 = env1->makeNewRail();
	r1->setPos(Point(10,10));
	Rail * r2 = env2->makeNewRail();
	r2->setPos(Point(10,9));
	r1->linkRailAt(Dir::W, r2);
	r2->linkRailAt(Dir::E, r1);
	Finder f;
	f.setDir(Dir::W);
	f.gotoRail(r1);
	RailVehicle * locomotive = new Locomotive;
	assert(f.isEmpty());
	assert(f.getRail()->getRailVehicle() == 0);
	f.getRail()->enter(locomotive);
	assert(!f.isEmpty());
	assert(f.getRail()->getRailVehicle() == locomotive);
	assert(f.getRail()->getRailVehicle()->getRail() == r1);

	Train * train = new Train;
	train->addVehicle(locomotive);

	delete train;
	delete locomotive;
END_TEST
	}
};

MAKE_LOGGER(TestSuite1);
int main() {
	PropertyConfigurator::doConfigure("log4cplus.properties");
	try {
		/*
		Test::Suite ts;
		ts.add(auto_ptr<Test::Suite>(new TestSuite1));

		auto_ptr<Test::Output> output(new Test::CompilerOutput);
		ts.run(*output, true);
		*/

		Test::TextOutput output(Test::TextOutput::Verbose);
		TestSuite1 t;
		return t.run(output) ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	catch (...) {
		cout << "unexpected exception encountered\n";
	}
}
