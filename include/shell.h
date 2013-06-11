/**********************************
*
* Shell
*
***********************************/

#ifndef _shell
#define _shell_

#define WHTBLK 	0xF0 // Atributo de video. Fondo Blanco - Letras Negras
#define REDWHT 	0x4F // Atributo de video. Fondo Rojo - Letras Blancas

#define SCREEN		0xb8000
#define ROWS		25
#define COLS		80

#define ESC 		-27
#define LEFT		-28
#define RIGHT		-29
#define UP			-30
#define DOWN		-31
#define MAY			0
#define MIN			0
#define CAPS		-34

/* Muestra la imagen de inicio */
void showSplashScreen();

/* Tiempo de espera */
void wait(int time);

/* Limpia la pantalla */
void k_clear_screen();

/* Inicializa el shell */
void initializeShell();

/* Escribe en la pantalla */
void flush();

/* Devuelve el caracter indicado por el scancode */
void decodeScancode(int scancode);

/* Imprime el prompt del sistema */
void printPrompt();

/* Devuelve la posicion en la pantalla */
char * screenPosition(int advance);

/* Mueve las lineas del shell una linea hacia arriba */
void moveUp();

/* Actualiza el cursor */
void update_cursor(int row, int col);

/* Interpreta un comando ingresado */
void interpret();

/* Analiza el scancode, moviendo el cursor y seleccionando el teclado */
int defineScancode(int scancode);

int cursorRange(int left, int right);

void printKey(char c);

void putOnTopRight(unsigned char * value);

#endif
