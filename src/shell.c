#include "../include/defs.h"
#include "../include/shell.h"
#include "../include/kernel.h"
#include "../include/kasm.h"
#include "../include/string.h"
#include "../include/stdio.h"

/* Imprime el prompt del sistema */
void printPrompt();

/* Mueve las lineas del shell una linea hacia arriba */
void moveUp();

/* Interpreta un comando ingresado */
void interpret();

/* Analiza el scancode, moviendo el cursor y seleccionando el teclado */
int defineScancode(int scancode);

void newLine();

void backspace();

void newKey(char c);

void tab();

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

unsigned char mayKeys[] =
{  0,  ESC, '!', '@', '#', '$', '%', '^', '&', '*', 
 '(', ')', '_', '+','\b','\t', 'Q', 'W', 'E', 'R', 
 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}','\n',   0,
 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
'"', '~',   MIN,'|', 'Z', 'X', 'C', 'V', 'B', 'N',
 'M', '<', '>', '?',   MIN, '*',   0, ' ',   CAPS,	0,
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
int tickpos = 0;
int promptLength;

unsigned char screen[ROWS][COLS]; /* screen[filas][columnas]*/
int curRow = 1;
int curCol = 0;

/***************************************************************
*initializeShell
*
* Inicializa el shell
****************************************************************/

void initializeShell() {
	char * vidmem = (char *) SCREEN;
	unsigned int i = 0;
	
	while(i < (ROWS*COLS*2)) {
		if(i <= COLS*2)
			vidmem[i++] = REDWHT;
		else
			vidmem[i++] = WHTBLK;
	}
	
	printPrompt();
	update_cursor(curRow, ++curCol);
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
       __write(STDOUT, &(screen[i]), COLS);
    }
}

/***************************************************************
*printKey
*
* Imprime una tecla en pantalla
****************************************************************/

void decodeScancode(int scancode) {
	char c;
	
	if(scancode <= 80) {
		if(defineScancode(scancode))
			return;
			
		c = keyboard[scancode];
		
		if(c != 0)
			printKey(c);
    }
}


void printKey(char c) {
	int i;
		
	switch(c) {
	
		case '\n':
					newLine();
					break;
		case '\b':
					backspace();
					break;
		case '\t':
					tab();
					break;
		default:
					newKey(c);
	}

}

/***************************************************************
*interpret
*
* Interpreta un comando ingresado 
****************************************************************/

void interpret() {
	const unsigned char * command = &(screen[curRow][promptLength + 1]);
	
	if(strcmp( (char *) command, "") == 0) {
		return;
	}
	
	if(strcmp( (char *) command, "clear") == 0) {
		k_clear_screen();
		curRow--;
		return;
	}
	
	curRow++;
	curCol = 0;
	if(strcmp( (char *) command, "lspci") == 0) {
		moveUp();
		lspci();
		return;
	}
	
	if(strcmp( (char *) command, "hour") == 0) {
		moveUp();
		printf("La hora es %s", read_rtc());
		return;
	}
	
	printf("Command not found");
	
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
		screen[curRow][curCol++] = prompt[i++];
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
				return TRUE;
		case RIGHT:
				return TRUE;
		case CAPS:
				if(keyboard == minKeys)
					keyboard = mayCapsKeys;
				else
					keyboard = minKeys;
				return TRUE;
		case ESC:
				while(screen[curRow][curCol-1] != 0) {
					backspace();
				}
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
	
	if(curRow > ROWS-1){
		curRow = ROWS-1;

		for(i = 1; i < ROWS-1; i++) {
			for(j = 0; j < COLS; j++){
				screen[i][j] = screen[i+1][j];
			}
		}
		
		for(j = 0; j < COLS; j++){
			screen[i][j] = 0;
		}
	}
}

/***************************************************************
*screenPosition
*
* Devuelve la posicion de la pantalla en donde escribir
****************************************************************/

char * screenPosition(int advance) {
		char * video = (char *) SCREEN;
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

    for(i = 1; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            screen[i][j] = 0;
        }
    }

	curRow = 1;
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
    unsigned short position = row * COLS + col; 

	_outb( (unsigned char *) 0x03D4, 0x0E);
	_outb( (unsigned char *) 0x03D5, position >> 8);
	_outb( (unsigned char *) 0x03D4, 0x0F);
	_outb( (unsigned char *) 0x03D5, position);
 }
 
/***************************************************************
*newLine
*
*
* Imprime una nueva linea
****************************************************************/

void newLine() {
	interpret();
	curRow++;
	moveUp();
	curCol = 0;
	printPrompt();
	flush();
	update_cursor(curRow, ++curCol);
}

void backspace() {
	int auxCol = curCol;
	int auxRow = curRow;

	if(--curCol == -1) {
		curCol = COLS-1;
		curRow--;
	}
	
	if(screen[curRow][curCol] != 0) {
		screen[curRow][curCol] = 0;
	} else {
		curCol = auxCol;
		curRow = auxRow;
	}
	
	flush();
	update_cursor(curRow, curCol);
}

void tab() {
	int i;
	
	for(i = 0; i < 8 ; i++)
		newKey(' ');

}


void newKey(char c) {
	screen[curRow][curCol] = c;
	if(++curCol > COLS-1){
		curRow++;
		curCol = 0;
		moveUp();
	}
	update_cursor(curRow, curCol);
	flush();
}

void putOnTopRight(unsigned char * value) {
	int i, aRow = 0, aCol = COLS-10;
	
	for(i = 0; value[i] != 0; i++) {
		screen[aRow][aCol++] = value[i];
	}
	flush();
}