#include "../include/kc.h"


int tickpos = 0;
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
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
}

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

/***************************************************************
*print
* Imprime por pantalla un string
*
*Recibe: String
****************************************************************/

void print (char* string){
    int counter = 0;
    char *video = (char *) 0xb8000;	

    while (string[counter] != '\0'){
    	
    	video[tickpos]=string[counter];
    	tickpos += 2;
    	counter++;

    }
   
}

void printChar(char c) {
    char *video = (char *) 0xb8000;
    video[tickpos+=2] = c;
	
}

void printInt (int num){
    if(num < 0) {
	printChar('-');
	num = num * -1;
    }

    if(num < 10) {
	printChar('0'+ num);
	return;
    }

    printInt(num/10);
    printChar('0'+ (num % 10));

}

/***************************************************************
*Imprime el teclado
****************************************************************/

char getKey(int scancode) {
	return kbdus[scancode];
}


void backspace() {
	print(" ");
	tickpos -= 4;
}


void int_09(int scancode) {
    if(scancode == 14) {
	backspace();
    }

    if(scancode <= 128 && scancode != 14) {
	    char *video = (char *) 0xb8000;
	    video[tickpos+=2] = getKey(scancode);
    }
}

