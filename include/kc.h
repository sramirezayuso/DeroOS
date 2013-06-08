/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro
#define CYAN_TXT 0x72 // Atributo de video. Letras blancas, fondo negro

/* Muestra la imagen de inicio */
void showSplashScreen();

/* Tiempo de espera */
void wait(int time);

/* Limpia la pantalla */
void k_clear_screen();

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

/* Escribe en la pantalla */
void flush();
			 
/* Devuelve el caracter indicado por el scancode */
char getKey(int scancode);

/* Devuelve la posicion en la pantalla */
char * screenPosition(int advance);

#endif
