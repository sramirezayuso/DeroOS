#include "../include/defs.h"
#include "../include/shell.h"
#include "../include/kernel.h"
#include "../include/kasm.h"
#include "../include/string.h"

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

/***************************************************************
*initializeShell
*
* Inicializa el shell
****************************************************************/

void initializeShell() {
	char * vidmem = (char *) 0xb8000;
	unsigned int i=0;
	
	while(i < (80*25*2)) {
		vidmem[i]=BLACK_TXT;
		i++;
	}
	
	printPrompt();
	update_cursor(curLine, curCol);
	curCol++;
	flush();
}

/***************************************************************
*flush
*
* Refresca la pantalla
****************************************************************/

void flush() {
	int i;

	tickpos = 0;
    for(i = 0; i < ROWS; i++){
       __write(STDOUT, &(screen[i]), 80);
    }
}

/***************************************************************
*printKey
*
* Imprime una tecla en pantalla
****************************************************************/

void printKey(int scancode) {
    int i, j;
	char c = kbdus[scancode];;
	
	if(c == 0)
		return;
	
    if(scancode <= 128) {
		moveUp();
		
		switch(c) {
		
		case '\n':
					screen[curLine][curCol] = 0;
					interpret();
					curLine++;
					moveUp();
					curCol = 0;
					printPrompt();
					break;
		case '\b':
					if(curCol > promptLength + 1) { 
						screen[curLine][--curCol] = ' ';
					}
					curCol--;
					break;
		case '\t':
					for(i = 0; i < 5; i++)
						screen[curLine][curCol++] = ' ';
					break;
		default:
				    screen[curLine][curCol] = c;
		}
		
        flush();
		update_cursor(curLine, curCol);
        curCol++;
        if(curCol > COLS-1){
            curLine++;
            curCol = 0;
        }
    }
}

/***************************************************************
*interpret
*
* Interpreta un comando ingresado 
****************************************************************/

void interpret() {
	const unsigned char * command = &(screen[curLine][promptLength + 1]);
	
	if(strcmp( (char *) command, "clear") == 0) {
		k_clear_screen();
		curLine--;
	}


}

/***************************************************************
*printPrompt
*
* Imprime el prompt
****************************************************************/
void printPrompt() {
	const char * prompt = "DERO9000$>"; 
	int i = 0;

	promptLength = strlen(prompt);
	while(prompt[i] != 0) {
		screen[curLine][curCol++] = prompt[i++];
	}
}

/***************************************************************
*moveUp
*
* Mueve las lineas del shell una linea hacia arriba
****************************************************************/
void moveUp() {
	int i, j;
	
	if(curLine > ROWS-1){
		curLine = ROWS-1;

		for(i = 0; i < ROWS; i++) {
			for(j = 0; j < COLS; j++){
				screen[i][j] = screen[i+1][j];
			}
		}
	}
}

/***************************************************************
*screenPosition
*
* Devuelve la posicion de la pantalla en donde escribir
****************************************************************/

char * screenPosition(int advance) {
		char * video = (char *) 0xb8000;
	    char * ret = video + tickpos;
		tickpos += advance;
		return ret;
}


/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() {
	int i, j;

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            screen[i][j] = ' ';
        }
    }

	curLine = 0;
	curCol = 0;
	flush();
}


/***************************************************************
*update_cursor
*
* http://wiki.osdev.org/Text_Mode_Cursor
* by Dark Fiber
****************************************************************/

void update_cursor(int row, int col)
 {
    unsigned short position = row * COLS + col + 1; 

	_outb( (unsigned char *) 0x03D4, 0x0E);
	_outb( (unsigned char *) 0x03D5, position >> 8);
	_outb( (unsigned char *) 0x03D4, 0x0F);
	_outb( (unsigned char *) 0x03D5, position);
 }

