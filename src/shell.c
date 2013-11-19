#include "../include/defs.h"
#include "../include/shell.h"
#include "../include/kernel.h"
#include "../include/kasm.h"
#include "../include/string.h"
#include "../include/stdio.h"
#include "../include/kc.h"

/* Imprime el prompt del sistema */
void printPrompt();

/* Mueve las lineas del shell una linea hacia arriba */
void moveUp();

/* Interpreta un comando ingresado */
void interpret();

/* Analiza el scancode, moviendo el cursor y seleccionando el teclado */
int defineScancode(int scancode);

void newLine();

void newLinePrompt();

void backspace();

void newKey(char c);

void tab();

void blink();

int tickpos;

unsigned char minKeys[] =
{  0,  ESC, '1', '2', '3', '4', '5', '6', '7', '8',
 '9', '0', '-', '=','\b','\t', 'q', 'w', 'e', 'r',
 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER,   0,
 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
'\'', '`',   MAY,'\\', 'z', 'x', 'c', 'v', 'b', 'n',
 'm', ',', '.', '/',   MAY, '*',   0, ' ',   CAPS,	0,
  0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 	UP,   0,  '-', LEFT,  0, RIGHT, '+', 0,
 DOWN,   0,    0,   0,   0,   0,   0,   0,   0,
};

unsigned char mayKeys[] =
{  0,  ESC, '1', '2', '3', '4', '5', '6', '7', '8',
 '9', '0', '-', '=','\b','\t', 'Q', 'W', 'E', 'R',
 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', ENTER,   0,
 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
'\'', '`',   MIN,'\\', 'Z', 'X', 'C', 'V', 'B', 'N',
 'M', ',', '.', '/',   MIN, '*',   0, ' ',   CAPS,	0,
  0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 	UP,   0,  '-', LEFT,  0, RIGHT, '+', 0,
 DOWN,   0,    0,   0,   0,   0,   0,   0,   0,
};

unsigned char shiftMinKeys[] =
{  0,  ESC, '!', '@', '#', '$', '%', '^', '&', '*',
 '(', ')', '_', '+','\b','\t', 'Q', 'W', 'E', 'R',
 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER,   0,
 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
'|', '"',   MIN,'\\', 'Z', 'X', 'C', 'V', 'B', 'N',
 'M', '<', '>', '?',   MIN, '*',   0, ' ',   CAPS,	0,
  0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 	UP,   0,  '-', LEFT,  0, RIGHT, '+', 0,
 DOWN,   0,    0,   0,   0,   0,   0,   0,   0,
};
unsigned char shiftMayKeys[] =
{  0,  ESC, '!', '@', '#', '$', '%', '^', '&', '*',
 '(', ')', '_', '+','\b','\t', 'q', 'w', 'e', 'r',
 't', 'y', 'u', 'i', 'o', 'p', '{', '}', ENTER,   0,
 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',
'|', '"',  MAY,'\\', 'z', 'x', 'c', 'v', 'b', 'n',
 'm', '<', '>', '?',   MAY, '*',   0, ' ',   CAPS,	0,
  0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 	UP,   0,  '-', LEFT,  0, RIGHT, '+', 0,
 DOWN,   0,    0,   0,   0,   0,   0,   0,   0,
};

unsigned char * keyboard = minKeys;
int promptLength;

unsigned char screen[ROWS][COLS]; /* screen[filas][columnas]*/
int curRow = 1;
int curCol = 0;
int promptRow, promptCol;

/***************************************************************
*initializeShell
*
* Inicializa el shell
****************************************************************/

void initializeShell() {
	int redwht = REDWHT;
	int blkwht = BLKWHT;
	unsigned int i = 0;

	while(i < (ROWS*COLS*2)) {
		tickpos = i;
		if(i <= COLS*2)
			__write(STDOUT, &redwht, 1);
		else
			__write(STDOUT, &blkwht, 1);
		i++;
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
    } else if (scancode == 170 || scancode == 182){
        if (keyboard == shiftMinKeys)
            keyboard = minKeys;
        else if (keyboard == shiftMayKeys)
            keyboard = mayKeys;
        return TRUE;
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
	const unsigned char * input = &(screen[promptRow][promptLength + 1]);
	char * aux, * command, c;

	command = "";
	if(strcmp( (char *) input, command) == 0) {
		return;
	}

	command = "clear";
	if(strcmp( (char *) input, command) == 0) {
		k_clear_screen();
		curRow--;
		return;
	}

	command = "blink";
	if(strcmp( (char *) input, command) == 0) {
		blink();
		return;
	}

	curRow++;
	curCol = 0;
	command = "help";
	if(strcmp( (char *) input, command) == 0) {
		moveUp();
		printf("clear - Clears the screen.\n");
		printf("lspci - Shows the devices connected to the PCI buses.\n");
		printf("hour - Exhibits the current time.\n");
		printf("cputemp - Displays the cpu temperature.\n");
		printf("raisetemp - Raises the cpu temperature.\n");
		printf("blink - Changes the back of the screen and makes it blink.\n");
		printf("putc [param] - Acts as the putc function.\n");
		printf("getc [param] - Acts as the getc function.\n");
		printf("printf [param] - Acts as the printf function.\n");
		printf("scanf -x [param] - Acts as the scanf function.\n");
		printf("help - It's what you are reading.");
		return;
	}

	command = "lspci";
	if(strcmp( (char *) input, command) == 0) {
		moveUp();
		lspci();
		return;
	}

	command = "hour";
	if(strcmp( (char *) input, command) == 0) {
		moveUp();
		printf("The hour is %s", read_rtc());
		return;
	}

	command = "cputemp";
	if(strcmp( (char *) input, command) == 0) {
		moveUp();
		printf("The temperature is %s, %s under max", read_temp(), rel_temp());
		return;
	}

	command = "raisetemp";
	if(strcmp( (char *) input, command) == 0) {
		raise_temp();
		return;
	}

	command = "getc ";
	aux = strstarts( (char *) input, command);
	if(aux != NULL) {
		moveUp();
		promptCol = promptLength + strlen(command) + 1;
		c = getc();
		putc(c);
		return;
	}

	command = "putc ";
	aux = strstarts( (char *) input, command);
	if(aux != NULL) {
		moveUp();
		putc(screen[promptRow][promptLength + strlen(command) + 1]);
		return;
	}

	command = "printf ";
	aux = strstarts( (char *) input, command);
	if(aux != NULL) {
		moveUp();
		char s[BUFF];
		promptCol = promptLength + strlen(command) + 1;
		scanf("%s", s);
		printf("%s",s);
		return;
	}

	command = "scanf -d ";
	aux = strstarts( (char *) input, command);
	if(aux != NULL) {
		moveUp();
		int d;
		promptCol = promptLength + strlen(command) + 1;
		scanf("%d",&d);
		printf("%d",d);
		return;
	}

	command = "scanf -c ";
	aux = strstarts( (char *) input, command);
	if(aux != NULL) {
		moveUp();
		char c;
		promptCol = promptLength + strlen(command) + 1;
		scanf("%c",&c);
		printf("%c",c);
		return;
	}

	command = "scanf -s ";
	aux = strstarts( (char *) input, command);
	if(aux != NULL) {
		moveUp();
		char s[BUFF];
		promptCol = promptLength + strlen(command) + 1;
		scanf("%s", s);
		printf("%s",s);
		return;
	}

	moveUp();
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
	promptRow = curRow;
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
        case MIN:
                if(keyboard == mayKeys)
					keyboard = shiftMayKeys;
                else if(keyboard == minKeys)
					keyboard = shiftMinKeys;
				return TRUE;
		case CAPS:
				if(keyboard == minKeys)
					keyboard = mayKeys;
				else
					keyboard = minKeys;
				return TRUE;
		case ESC:
				while(screen[curRow][curCol-1] != 0) {
					backspace();
				}
				return TRUE;
		case ENTER:
				newLinePrompt();
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
		promptRow--;
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
* Fuente: http://wiki.osdev.org/Text_Mode_Cursor
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
	curRow++;
	moveUp();
	curCol = 0;
	flush();
	update_cursor(curRow, curCol);
}

void newLinePrompt() {
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

void putOnTopRight(unsigned char * value1, unsigned char * value2) {
	int i, aRow = 0, aCol = COLS-16;

	for(i = 0; value1[i] != 0; i++) {
		screen[aRow][aCol++] = value1[i];
	}

	screen[aRow][aCol++]= ' ';
	screen[aRow][aCol++]= '|';
	screen[aRow][aCol++]= ' ';

	for(i = 0; value2[i] != 0; i++) {
		screen[aRow][aCol++] = value2[i];
	}
	flush();
}

char readFromShell() {
	char ret = screen[promptRow][promptCol];

	if(++promptCol == COLS) {
		promptCol = 0;
		promptRow++;
	}

	return ret;
}

int prev=TRUE;
void blink() {
	int back = prev==TRUE?WHTBLK:BLKWHT;
	unsigned int i = COLS*2 + 1;

	while(i < (ROWS*COLS*2)) {
		tickpos = i++;
		__write(STDOUT, &back, 1);
	}

	prev = prev==TRUE?FALSE:TRUE;
	flush();
}
