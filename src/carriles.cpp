/*davidpresas msn
 * (l)imi15
 * donverony
 * swimg
 * evaforeverkty*/

// revisar funcion int Puede_Haber_Cambio_De_Direccion ()
//falla al hacer curvas con alguna al lado
//hay una funcion repetida.......

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

//DEFINICIONES PARA TAMAÑO A MOSTRAR POR PANTALLA
#define X_MIN 2
#define Y_MIN 2
#define X_MAX 60
#define Y_MAX 30

//DEFINICIONES PARA TAMAÑO DE ESCENA
#define FILAS 1+Y_MAX-Y_MIN
#define COLUMNAS 1+X_MAX-X_MIN

#define PRIMERA_FILA 0
#define ULTIMA_FILA FILAS-1
#define PRIMERA_COLUMNA 0
#define ULTIMA_COLUMNA COLUMNAS-1

#define NOMBRE_FICHERO "mapa.txt"
#define SIMBOLO_VACIO ' '
#define NUMERO_DE_DIRECCIONES 8
#define MEDIA_DE_DIRECCIONES NUMERO_DE_DIRECCIONES/2
#define CANTIDAD_PARA_DIRECCION_DEL_LADO NUMERO_DE_DIRECCIONES/4

char Mapa [FILAS][COLUMNAS];

enum DIRECCIONES {NOROESTE=0, NORTE, NORESTE, ESTE, SUDESTE, SUR, SUDOESTE, OESTE, ORIGEN};
enum CRUCES {CURVA = 0, CAMBIO_SENTIDO, CRUCE_VIAS};

int Direccion_Actual, Direccion_Anterior;

char Simbolos_Carriles [8] = {'\\', '|', '/', '-', '\\', '|', '/', '-'};
char Simbolos_Cruces [3] = {'.', '+', 'x'};

enum DIRECCION_GIRO {NINGUNO=0, IZQUIERDA=-1, DERECHA=1};

int Cambio_Direccion;

char Alrededor [3][3];

enum MODOS {RECORRIDO, ESCRITURA} Modo;

int Posicion_X = X_MIN, Posicion_Y = Y_MIN;

void Inicializacion ();
void Finalizacion();
void Muestra_Opciones ();
void Imprimir_Cadena (char *);
void Imprimir_Caracter (char);
void Refresco_De_Pantalla ();
void Imprimir_En_La_Posicion (int,int,char *);
void Posicionarse_En (int, int);
void Cambiar_Modo();

void Cargar_Alrededores();

void Modo_Recorrido ();
void Desplazarse (int dir);

void Modo_Escritura ();
void Cambiar_Direccion (int dir);
void Previsualizar_Direccion ();
int Se_Puede_Insertar ();
void Siguiente_Posicion ();
void Anterior_Posicion ();

void Insertar ();
char Buscar_Simbolo_A_Insertar();
char Buscar_Simbolo_De_Cruce();
char Buscar_Simbolo_Recto();
int Hay_Rieles_Solo_En (int dir1, int dir2);
int Esta_Vacio_En (int dir);
int Hay_Algun_Tipo_De_Cruce_En (int dir);
int Hay_Cambio_De_Sentido_En (int dir);
int Hay_Curva_En (int dir);
int Hay_Cruce_En (int dir);
int Hay_Simbolo_Igual_A_La_Direccion (int dir);
int Direccion_Contraria_A (int dir);
void Asignar_Posicion_De_Direccion (int dir, int &pos_x, int &pos_y);

void Grabar_Mapa (FILE *file);
int Se_Puede_Cambiar_De_Direccion (int);
int Se_Puede_Seguir (int );
int Se_Puede_Seguir_Recto();
int Estan_Vacios_Hacia_El_Excepto (int dir);
int Sumar_Direccion (int dir, int num);
int Restar_Direccion (int dir, int num);
int Puede_Haber_Cambio_De_Direccion ();

int Hay_Railes_Hacia_El (int dir);
int Hay_Railes_Cercanos ();
int Hay_Rail_Diferente_A_Direccion_Anterior_En (int dir);
int Hay_Rail_Diferente_A_Direccion_Actual_En (int dir);
int Hay_Algun_Tipo_De_Cruce_Alrededor ();

void Eliminar ();
void Vaciar_Mapa ();

char Simbolo_A_Insertar (int, int, int);

void Ver_Alrededores(int, int);
int Es_Par (int);

int main ()
{
	Inicializacion ();
	Muestra_Opciones ();

	Vaciar_Mapa();
	Direccion_Actual = ESTE;
	Modo = ESCRITURA;

	for(;;)
	{
		Cambiar_Modo();	
	}
	Finalizacion();
	return 0;
}

void Inicializacion ()
{
	initscr();
	raw();
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
}

void Finalizacion()
{
	endwin();
}

void Muestra_Opciones ()
{
	char frase[100];
	
	Imprimir_En_La_Posicion (30, Y_MAX+5, "CTRL-C PARA TERMINAR");
	
	Imprimir_En_La_Posicion (10, Y_MAX+5, "                     ");
	sprintf(frase, "X = %d, Y = %d", Posicion_X, Posicion_Y);
	Imprimir_En_La_Posicion (10, Y_MAX+5, frase);
	
	Imprimir_En_La_Posicion (10, Y_MAX+6, "MODO RECORRIDO = r   MODO ESCRITURA = e");

	if (Modo == RECORRIDO)
		Imprimir_En_La_Posicion (20, Y_MAX+7, "MODO RECORRIDO");
	else
		Imprimir_En_La_Posicion (20, Y_MAX+7, "MODO ESCRITURA");

	Refresco_De_Pantalla ();
}

void Imprimir_En_La_Posicion (int x, int y, char *frase)
{
	Posicionarse_En (x, y);
	Imprimir_Cadena (frase);
}

void Posicionarse_En (int x, int y)
{
	move(y,x);
}

void Imprimir_Cadena (char *frase)
{
	printw("%s", frase);
}

void Imprimir_Caracter (char car)
{
	printw("%c", car);
}

void Refresco_De_Pantalla ()
{
	refresh();
}

void Cambiar_Modo ()
{
	if (Modo == RECORRIDO)
	{
		Modo = ESCRITURA;
		Muestra_Opciones();
		Modo_Escritura();
	}
	else
	{
		Modo = RECORRIDO;
		Muestra_Opciones();
		Modo_Recorrido();
	}
}

void Posicionarse_En_Pantalla()
{
	move (Posicion_Y, Posicion_X);
}

void Modo_Recorrido ()
{
	int tecla;

	Posicionarse_En_Pantalla();
	Refresco_De_Pantalla ();

	while ((tecla=getch())!='e')
	{	
		switch (tecla)
        	{
			case KEY_UP: 	Desplazarse (NORTE);
					break;
	
			case KEY_DOWN:	Desplazarse (SUR);
					break;
	
			case KEY_LEFT:	Desplazarse (OESTE);
					break;
	
			case KEY_RIGHT:	Desplazarse (ESTE);
					break;
		}
		Muestra_Opciones ();
		Posicionarse_En_Pantalla();
		Refresco_De_Pantalla ();
	}
}

void Desplazarse (int dir)
{

	switch (dir)
	{
		case NORTE: --Posicion_Y;
			break;
		case SUR: ++Posicion_Y;
			break;
		case ESTE: ++Posicion_X;
			break;
		case OESTE: --Posicion_X;
			break;
	}
	
	if (Posicion_Y < Y_MIN)
	{
		Posicion_Y = Y_MAX;
	}
	if (Posicion_Y > Y_MAX)
	{
		Posicion_Y = Y_MIN;
	}
	if (Posicion_X < X_MIN)
	{
		Posicion_X = X_MAX;
	}
	if (Posicion_X > X_MAX)
	{
		Posicion_X = X_MIN;
	}
}

void Modo_Escritura ()
{
	int tecla;
	FILE *mapa;

	Direccion_Anterior = Direccion_Actual;

	Previsualizar_Direccion ();
	Cambio_Direccion = NINGUNO;

	while ((tecla = getch()) != 'r')
	{
		Ver_Alrededores(Posicion_X, Posicion_Y);
		switch (tecla)
		{
			case KEY_UP:	Insertar();
					break;

			case KEY_DOWN: 	Eliminar ();
					break;

			case KEY_LEFT:	Cambiar_Direccion (IZQUIERDA);
					break;

			case KEY_RIGHT:	Cambiar_Direccion (DERECHA);
					break;
		}

		Posicionarse_En_Pantalla();
		
		Ver_Alrededores(Posicion_X, Posicion_Y);
		move (Posicion_Y,Posicion_X);
		refresh();
		Previsualizar_Direccion ();
		Refresco_De_Pantalla ();
	}
	
	mapa = fopen(NOMBRE_FICHERO, "w");
	Grabar_Mapa (mapa);
	fclose(mapa);

}

void Cambiar_Direccion (int cambio)
{
// NOROESTE, NORTE, NORESTE, ESTE, SUDESTE, SUR, SUDOESTE, OESTE

	int sig = Direccion_Actual;

	if (Cambio_Direccion == NINGUNO)
	{
		Cambio_Direccion = cambio;
		sig = Direccion_Actual + Cambio_Direccion;
		switch (sig)
		{
			case -1: Direccion_Actual = OESTE;
				break;
	
			case 8: Direccion_Actual = NOROESTE;
				break;
	
			default: Direccion_Actual = sig;
				break;
		}
	}
	else
	{
		switch (cambio)
		{
			case IZQUIERDA: if (Cambio_Direccion == DERECHA)
					{
						Direccion_Actual = Direccion_Anterior;
						Cambio_Direccion = NINGUNO;
					}
					break;
			case DERECHA:   if (Cambio_Direccion == IZQUIERDA)
					{
						Direccion_Actual = Direccion_Anterior;
						Cambio_Direccion = NINGUNO;
					}
					break;
		}
	}
}

void Previsualizar_Direccion ()
{
// NOROESTE, NORTE, NORESTE, ESTE, SUDESTE, SUR, SUDOESTE, OESTE
	Posicionarse_En_Pantalla();

	Imprimir_Caracter (Simbolos_Carriles[Direccion_Actual]);

	Posicionarse_En_Pantalla();
}

int Se_Puede_Insertar ()
{
// NOROESTE, NORTE, NORESTE, ESTE, SUDESTE, SUR, SUDOESTE, OESTE

	int retorno = 1;
	
	retorno = Puede_Haber_Cambio_De_Direccion ();

	if (Direccion_Anterior != Direccion_Actual)
	{
		retorno = Se_Puede_Cambiar_De_Direccion(retorno);
	}
	else
	{
		retorno = Se_Puede_Seguir_Recto();
	}

	retorno = Se_Puede_Seguir (retorno);

	if (Hay_Railes_Cercanos ())
		retorno = 0;
	
	return retorno;
}

int Se_Puede_Seguir_Recto ()
{
	int retorno = 1;

	Cargar_Alrededores();
	
	if (Hay_Algun_Tipo_De_Cruce_En (ORIGEN))
	{
		if ((Hay_Rail_Diferente_A_Direccion_Anterior_En (Direccion_Actual)) || (Esta_Vacio_En (Direccion_Actual)))
		{
        	if (!Estan_Vacios_Hacia_El_Excepto (Direccion_Contraria_A (Direccion_Actual)))
            	retorno = 0;
		}
	}
	else if ((Hay_Rail_Diferente_A_Direccion_Anterior_En (ORIGEN)) && (Hay_Algun_Tipo_De_Cruce_Alrededor()))
	{
			retorno = 0;
	}

	if (Es_Par (Direccion_Anterior))
	{
		int dir1 = Sumar_Direccion (Direccion_Anterior,1);
		int dir2 = Restar_Direccion (Direccion_Anterior,1);
		if ((Hay_Rail_Diferente_A_Direccion_Anterior_En (dir1) && (Hay_Rail_Diferente_A_Direccion_Anterior_En (dir2))))
				retorno = 0;
	}

	
	return retorno;
}

int Hay_Railes_Cercanos ()
{
	int dir1, dir2;
	Cargar_Alrededores();

	dir1 = Sumar_Direccion (Direccion_Actual, CANTIDAD_PARA_DIRECCION_DEL_LADO);
	dir2 = Restar_Direccion (Direccion_Actual, CANTIDAD_PARA_DIRECCION_DEL_LADO);

	if ((Es_Par (Direccion_Actual)) && (Direccion_Actual == Direccion_Anterior))
	{
		if ((!Hay_Rail_Diferente_A_Direccion_Actual_En (dir1)) || (!Hay_Rail_Diferente_A_Direccion_Actual_En (dir2))) 
			return 1;
	}
	else if (Hay_Railes_Hacia_El (Direccion_Actual))
		return 1;

	return 0;
}

int Hay_Railes_Hacia_El (int dir)
{
	int dir1, dir2;
	
	dir1 = Sumar_Direccion (dir, 1);
	dir2 = Restar_Direccion (dir, 1);

	if ((Hay_Algun_Tipo_De_Cruce_En (dir1)) || (Hay_Algun_Tipo_De_Cruce_En (dir2)) || 
			(!Hay_Rail_Diferente_A_Direccion_Actual_En (dir1)) || (!Hay_Rail_Diferente_A_Direccion_Actual_En (dir2)))	
		return 1;
	return 0;

}

int Es_Par (int x)
{
	if (x % 2 == 0)
		return 1;
	return 0;
}

int Hay_Rail_Diferente_A_Direccion_Anterior_En (int dir)
{
	int pos_x, pos_y;

	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if ((Alrededor [pos_y][pos_x] != Simbolos_Carriles[Direccion_Anterior]) && (Alrededor [pos_y][pos_x] != SIMBOLO_VACIO))
		return 1;

	return 0;
}

int Hay_Rail_Diferente_A_Direccion_Actual_En (int dir)
{
	int pos_x, pos_y;

	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if (Alrededor [pos_y][pos_x] != Simbolos_Carriles[Direccion_Actual])
		return 1;

	return 0;
}

int Hay_Algun_Tipo_De_Cruce_Alrededor ()
{
	int retorno = 0;
	for (int i=0; i<NUMERO_DE_DIRECCIONES;++i)
	{
		if (Hay_Algun_Tipo_De_Cruce_En (i))
			retorno = 1;
	}
	return retorno;
}


void Cargar_Alrededores()
{
	int pos_x = Posicion_X - X_MIN;
	int pos_y = Posicion_Y - Y_MIN;
	
	Alrededor[0][0] = Mapa [pos_y-1] [pos_x-1];
	Alrededor[0][1] = Mapa [pos_y-1] [pos_x];
	Alrededor[0][2] = Mapa [pos_y-1] [pos_x+1];
	Alrededor[1][0] = Mapa [pos_y]   [pos_x-1];
	Alrededor[1][1] = Mapa [pos_y]   [pos_x];
	Alrededor[1][2] = Mapa [pos_y]   [pos_x+1];
	Alrededor[2][0] = Mapa [pos_y+1] [pos_x-1];
	Alrededor[2][1] = Mapa [pos_y+1] [pos_x];
	Alrededor[2][2] = Mapa [pos_y+1] [pos_x+1];

	if (pos_x == PRIMERA_COLUMNA)
	{
		Alrededor [0][0] = SIMBOLO_VACIO;
		Alrededor [1][0] = SIMBOLO_VACIO;
		Alrededor [2][0] = SIMBOLO_VACIO;
	}
	if (pos_x == ULTIMA_COLUMNA)
	{
		Alrededor [0][2] = SIMBOLO_VACIO;
		Alrededor [1][2] = SIMBOLO_VACIO;
		Alrededor [2][2] = SIMBOLO_VACIO;
	}
	if (pos_y == PRIMERA_FILA)
	{
		Alrededor [0][0] = SIMBOLO_VACIO;
		Alrededor [0][1] = SIMBOLO_VACIO;
		Alrededor [0][2] = SIMBOLO_VACIO;
	}
	if (pos_y == ULTIMA_FILA)
	{
		Alrededor [2][0] = SIMBOLO_VACIO;
		Alrededor [2][1] = SIMBOLO_VACIO;
		Alrededor [2][2] = SIMBOLO_VACIO;
	}
}

int Puede_Haber_Cambio_De_Direccion ()
{
	int retorno = 1;

	Cargar_Alrededores();
	
//	if (((Esta_Vacio_En (Direccion_Actual)) && (Hay_Algun_Tipo_De_Cruce_En (ORIGEN)))
//		|| ((Direccion_Actual != Direccion_Anterior) && (Esta_Vacio_En (Direccion_Actual))))
	
	if (Hay_Algun_Tipo_De_Cruce_En (ORIGEN))
	{
		if ((Esta_Vacio_En (Direccion_Actual)) && (!Estan_Vacios_Hacia_El_Excepto (Direccion_Contraria_A (Direccion_Anterior))))
			retorno = 0;
	}
	else if ((Esta_Vacio_En (Direccion_Actual)) && (Hay_Rail_Diferente_A_Direccion_Anterior_En (ORIGEN)))
	{
		retorno = 0;
	}










	return retorno;
}

int Estan_Vacios_Hacia_El_Excepto (int dir)
{
	int retorno = 1;
	int dir1, dir2, dir3, dir4;

	dir1 = Restar_Direccion (dir, 2);
	dir2 = Restar_Direccion (dir, 1);
	dir3 = Sumar_Direccion (dir, 1);
	dir4 = Sumar_Direccion (dir, 2);

	if (Esta_Vacio_En (dir))
		retorno = 0;
	
	if (!Esta_Vacio_En (dir1))
		retorno = 0;
	
	if (!Esta_Vacio_En (dir2))
		retorno = 0;

	if (!Esta_Vacio_En (dir3))
		retorno = 0;
	
	if (!Esta_Vacio_En (dir4))
		retorno = 0;

	return retorno;
}

int Restar_Direccion (int dir, int num)
{
	num = num % NUMERO_DE_DIRECCIONES;
	
	dir -= num;

	if (dir < 0)
	{
		dir = NUMERO_DE_DIRECCIONES + dir;
	}

	return dir;
}

int Sumar_Direccion (int dir, int num)
{
	num = num % NUMERO_DE_DIRECCIONES;
	
	dir += num;

	if (dir > 7)
	{
		dir -= NUMERO_DE_DIRECCIONES;
	}

	return dir;
}

int Se_Puede_Cambiar_De_Direccion (int a)
{
	int retorno = a;
	Cargar_Alrededores();

	if (Hay_Algun_Tipo_De_Cruce_Alrededor ())
		retorno = 0;

	if ((!Esta_Vacio_En(Direccion_Actual)) && (Esta_Vacio_En (ORIGEN)))
		retorno = 0;




























/*
	if ((Hay_Algun_Tipo_De_Cruce_En (Direccion_Anterior)) || (Hay_Algun_Tipo_De_Cruce_En (Direccion_Actual)))
		retorno = 0;

	if ((Hay_Algun_Tipo_De_Cruce_En (Direccion_Contraria_A (Direccion_Anterior))) || (Hay_Algun_Tipo_De_Cruce_En (Direccion_Contraria_A (Direccion_Actual))))
		retorno = 0;
*/
	return retorno; 
}

int Hay_Algun_Tipo_De_Cruce_En (int dir)
{
	if ((Hay_Curva_En (dir)) || (Hay_Cambio_De_Sentido_En (dir)) || (Hay_Cruce_En (dir)))
		return 1;
	return 0;

}

int Direccion_Contraria_A (int dir)
{
	dir = (dir + MEDIA_DE_DIRECCIONES) % NUMERO_DE_DIRECCIONES;
	return dir;
}

int Se_Puede_Seguir (int a)
{
	int Restric_Esquina_X_1 = 4, Restric_Esquina_Y_1 = 2;
	int Restric_Esquina_X_2 = 3, Restric_Esquina_Y_2 = 3;
	int Restric_Lateral = 2;	
	
	switch (Direccion_Actual)
	{	
		case NOROESTE: 	if ((Posicion_X == X_MIN) || (Posicion_Y == Y_MIN))
				{
					a = 0;
				} 
				else if ((Posicion_X <= X_MIN + Restric_Esquina_X_1) && (Posicion_Y <= Y_MIN + Restric_Esquina_Y_1))
				{
					a = 0;
				}
				else if ((Posicion_X <= X_MIN + Restric_Esquina_X_2) && (Posicion_Y <= Y_MIN + Restric_Esquina_Y_2))
				{
					a = 0;
				}
				break;	
		case NORTE: 	if (Posicion_Y <= Y_MIN + Restric_Lateral)
				{
					a = 0;
				}
				break;
		case NORESTE: 	if ((Posicion_X == X_MAX) || (Posicion_Y == Y_MIN))
				{
					a = 0;
				} 
				else if ((Posicion_X >= X_MAX - Restric_Esquina_X_1) && (Posicion_Y <= Y_MIN + Restric_Esquina_Y_1))
				{
					a = 0;
				}
				else if ((Posicion_X >= X_MAX - Restric_Esquina_X_2) && (Posicion_Y <= Y_MIN + Restric_Esquina_Y_2))
				{
					a = 0;
				}
				break;	
		case ESTE: 	if (Posicion_X >= X_MAX - Restric_Lateral)
				{
					a = 0;
				}
				break;
		case SUDESTE: 	if ((Posicion_X == X_MAX) || (Posicion_Y == Y_MAX))
				{
					a = 0;
				} 
				else if ((Posicion_X >= X_MAX - Restric_Esquina_X_1) && (Posicion_Y >= Y_MAX - Restric_Esquina_Y_1))
				{
					a = 0;
				}
				else if ((Posicion_X >= X_MAX - Restric_Esquina_X_2) && (Posicion_Y >= Y_MAX - Restric_Esquina_Y_2))
				{
					a = 0;
				}
				break;
		case SUR: 	if (Posicion_Y >= Y_MAX - Restric_Lateral)
				{
					a = 0;
				}
				break;
		case SUDOESTE: 	if ((Posicion_X == X_MIN) || (Posicion_Y == Y_MAX))
				{
					a = 0;
				} 
				else if ((Posicion_X <= X_MIN + Restric_Esquina_X_1) && (Posicion_Y >= Y_MAX - Restric_Esquina_Y_1))
				{
					a = 0;
				}
				else if ((Posicion_X <= X_MIN + Restric_Esquina_X_2) && (Posicion_Y >= Y_MAX - Restric_Esquina_Y_2))
				{
					a = 0;
				}
				break;
		case OESTE: 	if (Posicion_X <= X_MIN + Restric_Lateral)
				{
					a = 0;
				}
				break;
	}
	return a;
}


void Siguiente_Posicion ()
{
	switch (Direccion_Actual)
	{
		case NOROESTE:	--Posicion_X;

		case NORTE:	--Posicion_Y;
				break;

		case NORESTE:	--Posicion_Y;

		case ESTE:	++Posicion_X;
		    		break;

		case SUDESTE:	++Posicion_X;

		case SUR:	++Posicion_Y;
				break;

		case SUDOESTE:	++Posicion_Y;

		case OESTE:	--Posicion_X;
				break;
	}
}

void Anterior_Posicion ()
{
	switch (Direccion_Actual)
	{
		case NOROESTE:	++Posicion_X;
				
		case NORTE:	++Posicion_Y;
				break;

		case NORESTE:	++Posicion_Y;

		case ESTE:	--Posicion_X;
				break;

		case SUDESTE:	--Posicion_X;

		case SUR:	--Posicion_Y;
				break;

		case SUDOESTE:	--Posicion_Y;

		case OESTE:	++Posicion_X;
				break;
	}
}

void Vaciar_Mapa ()
{
    for (int i=0; i<=1+Y_MAX-Y_MIN; ++i)
    {
    	for (int j=0; j<=1+X_MAX-X_MIN; ++j)
	{
	    Mapa [i][j] = SIMBOLO_VACIO;
	}
    }
}

int Se_Puede_Insertar_Siguiente()
{













	return 1;
}

void Insertar ()
{
// NOROESTE, NORTE, NORESTE, ESTE, SUDESTE, SUR, SUDOESTE, OESTE
	char simbolo;
	
	if ((Se_Puede_Insertar()) && (Se_Puede_Insertar_Siguiente())) 
	{
		simbolo = Buscar_Simbolo_A_Insertar ();

		Posicionarse_En (Posicion_X,Posicion_Y);
		Imprimir_Caracter (simbolo);
		Refresco_De_Pantalla ();		
		
		move (32,30);
		printw ("%i  %i %c", Direccion_Anterior, Direccion_Actual, simbolo);
		refresh();	
		Mapa[Posicion_Y-Y_MIN][Posicion_X-X_MIN]  = simbolo;				
		
		Direccion_Anterior = Direccion_Actual;
		Siguiente_Posicion ();
		Cambio_Direccion = NINGUNO;
	}	
}

char Buscar_Simbolo_A_Insertar()
{
	char simbolo;

	if (Direccion_Anterior != Direccion_Actual)
		simbolo = Buscar_Simbolo_De_Cruce ();
	else 
		simbolo = Buscar_Simbolo_Recto();
	return simbolo;
}

char Buscar_Simbolo_De_Cruce ()
{
	char simbolo = SIMBOLO_VACIO;

	Cargar_Alrededores();

	if (Esta_Vacio_En (ORIGEN))
	{
		simbolo = Simbolos_Cruces [CURVA];
	}
	
	if (Hay_Curva_En (ORIGEN))
	{
		if (Hay_Rieles_Solo_En (Direccion_Contraria_A (Direccion_Anterior), Direccion_Actual))
			simbolo = Simbolos_Cruces [CURVA];
		else
			simbolo = Simbolos_Cruces [CAMBIO_SENTIDO];
	}
	else if ((Hay_Cambio_De_Sentido_En (ORIGEN)) || (!Esta_Vacio_En (ORIGEN)))
		simbolo = Simbolos_Cruces [CAMBIO_SENTIDO];
	else if (Hay_Cruce_En (ORIGEN))
		simbolo =  Simbolos_Cruces [CRUCE_VIAS];

	
	if (Hay_Rieles_Solo_En (Direccion_Contraria_A (Direccion_Anterior), Direccion_Actual))
		simbolo = Simbolos_Cruces [CURVA];


	return simbolo;
}

void Asignar_Posicion_De_Direccion (int dir, int &pos_x, int &pos_y)
{
	pos_x = 1;
	pos_y = 1;
	
	switch (dir)
	{
		case NOROESTE:	--pos_y; --pos_x;
				break;

		case NORTE:	--pos_y;
				break;

		case NORESTE:	--pos_y; ++pos_x;
				break;

		case ESTE:	++pos_x;
				break;

		case SUDESTE:	++pos_y; ++pos_x;
				break;

		case SUR:	++pos_y;
				break;

		case SUDOESTE:	++pos_y; --pos_x;
				break;

		case OESTE:	--pos_x;
				break;
	}
}

int Hay_Rieles_Solo_En (int dir1, int dir2)
{
	int retorno = 1;

	for (int x=0; x<8; ++x)
	{
		if ((x == dir1) || (x == dir2))
		{
			if (Esta_Vacio_En (x))
				retorno = 0;
		}
		else
		{
			if (!Esta_Vacio_En (x))
				retorno = 0;
		}
	}

	return retorno;
}

int Esta_Vacio_En (int dir)
{
	int pos_x, pos_y;

	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if (Alrededor [pos_y][pos_x] == SIMBOLO_VACIO)
		return 1;

	return 0;
}

char Buscar_Simbolo_Recto ()
{
	int simbolo = SIMBOLO_VACIO;

	Cargar_Alrededores();
		
	if (!Esta_Vacio_En (ORIGEN))
	{
		if ((Hay_Cambio_De_Sentido_En (ORIGEN)) || (Hay_Curva_En (ORIGEN)))
			simbolo = Simbolos_Cruces [CAMBIO_SENTIDO];

		else if ((Hay_Cruce_En (ORIGEN)) || (!Hay_Simbolo_Igual_A_La_Direccion (ORIGEN)))
			simbolo = Simbolos_Cruces [CRUCE_VIAS];
		else
			simbolo = Simbolos_Carriles[Direccion_Actual];
	}
	else
	{
		simbolo = Simbolos_Carriles[Direccion_Actual];
	}
	
	return simbolo;
}

int Hay_Curva_En (int dir)
{
	int pos_x, pos_y;
	
	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if (Alrededor [pos_y][pos_x] == Simbolos_Cruces[CURVA])
		return 1;

	return 0;
}

int Hay_Cambio_De_Sentido_En (int dir)
{
	int pos_x, pos_y;

	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if (Alrededor [pos_y][pos_x] == Simbolos_Cruces[CAMBIO_SENTIDO])
		return 1;

	return 0;
}

int Hay_Cruce_En (int dir)
{
	int pos_x, pos_y;

	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if (Alrededor [pos_y][pos_x] == Simbolos_Cruces[CRUCE_VIAS])
		return 1;

	return 0;
}

int Hay_Simbolo_Igual_A_La_Direccion (int dir)
{
	int pos_x, pos_y;

	Asignar_Posicion_De_Direccion (dir, pos_x, pos_y);

	if (Alrededor [pos_y][pos_x] == Simbolos_Carriles[Direccion_Actual])
		return 1;

	return 0;
}

void Eliminar ()
{
	Direccion_Anterior = Direccion_Actual;
	Anterior_Posicion ();
	Cambio_Direccion = NINGUNO;
	//seguir
}

void Ver_Alrededores(int x, int y)
{
	Cargar_Alrededores();
	
	move (10,65);
	printw (" %c x %c x %c", Alrededor[0][0], Alrededor[0][1], Alrededor[0][2]);
	refresh();
	move (11,65);
	printw (" %c  x   %c", Alrededor[1][0], Alrededor[1][2]);
	refresh();
	move (12,65);
	printw (" %c x %c x %c", Alrededor[2][0], Alrededor[2][1], Alrededor[2][2]);
	refresh();
}

void Grabar_Mapa (FILE *file)
{
	for (int i=PRIMERA_FILA; i<=ULTIMA_FILA; ++i)
	{
		for (int j=PRIMERA_COLUMNA; j<=ULTIMA_COLUMNA; ++j)
		{
			fprintf(file, "%c", Mapa[i][j]);
		}
		fprintf(file, "\n");
	}
}
