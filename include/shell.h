/**********************************
*
* Shell
*
***********************************/

#ifndef _shell
#define _shell_

#define BLACK_TXT 	0xF0 // Atributo de video. Fondo Blanco - Letras Negras

#define ESC 		-27
#define LEFT		-28
#define RIGHT		-29
#define UP			-30
#define DOWN		-31
#define MAY			-32
#define MIN			-33
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
void printKey(int scancode);

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

#endif
