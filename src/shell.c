#include "../include/defs.h"
#include "../include/shell.h"
#include "../include/kernel.h"
#include "../include/kasm.h"
#include "../include/string.h"

unsigned char minKeys[] =
{  0,  ESC, '1', '2', '3', '4', '5', '6', '7', '8', 
 '9', '0', '-', '=','\b','\t', 'q', 'w', 'e', 'r', 
 't', 'y', 'u', 'i', 'o', 'p', '[', ']','\n',   0,
 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
'\'', '`',   MAY,'\\', 'z', 'x', 'c', 'v', 'b', 'n',
 'm', ',', '.', '/',   MAY, '*',   0, ' ',   CAPS,	0,
  0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 	UP,   0,  '-', LEFT,  0, RIGHT, '+', 0,
 DOWN,   0,    0,   0,   0,   0,   0,   0,   0, 
};

unsigned char mayCapsKeys[] =
{  0,  ESC, '1', '2', '3', '4', '5', '6', '7', '8', 
 '9', '0', '-', '=','\b','\t', 'Q', 'W', 'E', 'R', 
 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']','\n',   0,
 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
'\'', '`',   MIN,'\\', 'Z', 'X', 'C', 'V', 'B', 'N',
 'M', ',', '.', '/',   MIN, '*',   0, ' ',   CAPS,	0,
  0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 	UP,   0,  '-', LEFT,  0, RIGHT, '+', 0,
 DOWN,   0,    0,   0,   0,   0,   0,   0,   0, 
};

unsigned char * keyboard = minKeys;


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
	char c;
	printInt(scancode);
	
	if(defineScancode(scancode))
		return;
	
	c = keyboard[scancode];
	if(c == 0)
		return;
	
    if(scancode <= 80) {
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
						if(cursorRange(promptLength+2, COLS-1)) { 
							screen[curLine][--curCol] = ' ';
						}
						curCol--;
						break;
			case '\t':
						for(i = 0; i < 5; i++)
							screen[curLine][curCol++] = ' ';
						break;
			default:
						if(!cursorRange(promptLength+1, COLS-2))
							return;
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
*defineScancode
*
* Analiza el scancode, moviendo el cursor y seleccionando el teclado
****************************************************************/
int defineScancode(int scancode) {
	char c = keyboard[scancode];
	
	switch(c) {
	
		case UP:
				return TRUE;
		case DOWN:
				return TRUE;
		case LEFT:
				if(cursorRange(promptLength+2, COLS-1)) {
					update_cursor(curLine, curCol-2);
					curCol--;
				}
				return TRUE;
		case RIGHT:
				if(cursorRange(promptLength+1,COLS-2)){
					update_cursor(curLine, curCol);
					curCol++;
				}
				return TRUE;
		case CAPS:
				if(keyboard == minKeys)
					keyboard = mayCapsKeys;
				else
					keyboard = minKeys;
				return TRUE;
		case MIN:
				keyboard = minKeys;
				return 0;
		case MAY:
				keyboard = minKeys;
				return 0;
		case ESC:
				while(curCol > promptLength + 1 ) {
					screen[curLine][--curCol] = ' ';
				}
				flush();
				update_cursor(curLine, curCol-1);
				return TRUE;
		default:
				return FALSE;
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
 
 /***************************************************************
*cursorRange
*
* 
****************************************************************/

int cursorRange(int left, int right) {
	return (curCol >= left) && (curCol <= right);
}
