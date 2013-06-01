#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/stdio.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */



void int_08() {

    //char *video = (char *) 0xb8000;
    //video[tickpos+=2]='!';


}


void int_09(int scancode) {

	if(scancode <= 128) {
		putchar( (int) getKey(scancode));
	}
}

size_t __write(int fd, const void* buffer, size_t count) {

	size_t cantW = 0;
	char * b = (char *) buffer;
	
	if(count < 0)
		return -1;

	switch(fd) {
	
		case 1:
				while(count) {
					_writeScreen(b[cantW], screenPosition(2));
					count--;
					cantW++;
				}
				
				return cantW;
		default:
				return -1;
	}
}



size_t __read(int fd, void* buffer, size_t count) {

	size_t cantR = 0;

	if(count < 0)
		return -1;

	switch(fd) {
	
		case 1:
				while(count) {
					_read(buffer + count, screenPosition());
					count--;
					cantR++;
				}
				
				return cantR;
		default:
				return -1;
	}

}


/**********************************************
kmain() 
Punto de entrada de código C.
*************************************************/

kmain() 
{

        int i, num;

/* Borra la pantalla. */ 

	k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

    setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	
/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
/* Habilito interrupcion de timer tick*/

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);
        
	_Sti();

        while(1)
        {
	//print("Hola");
        }
	
}

