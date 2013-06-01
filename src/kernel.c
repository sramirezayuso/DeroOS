#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/stdio.h"

DESCR_INT idt[0xA];                     /* IDT de 10 entradas*/
IDTR idtr;                              /* IDTR */



void int_08() {

    //char *video = (char *) 0xb8000;
    //video[tickpos+=2]='!';


}


void int_09(int scancode) {
<<<<<<< HEAD
    if (scancode == 2){
        int temp[10];
        __read(1, (void*) temp, 1);
        putchar(temp[0]);
    }
	
    if(scancode <= 128) {
=======
    if (scancode = 2){
        int[10] temp;
        __read(1, (void*) temp, 1)
        putchar(temp[0])
    }
	if(scancode <= 128) {
	    putchar(scancode);
>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df
		putchar( (int) getKey(scancode));
    }
}

size_t __write(int fd, const void* buffer, size_t count) {

<<<<<<< HEAD
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
=======
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
>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df
}



size_t __read(int fd, void* buffer, size_t count) {

<<<<<<< HEAD
        size_t cantR = 0;
        char* cBuf;
        if(count < 0)
                return -1;

        switch(fd) {

=======
	size_t cantR = 0;
	char* cBuf;
	if(count < 0)
		return -1;

	switch(fd) {

>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df
        case 1:
            while(count) {
                char var = _read(0xb8000);
                cBuf = buffer;
<<<<<<< HEAD
                cBuf[cantR] = var;
=======
                cBuf[count] = var;
>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df
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

void kmain()
{

        int i, num;

/* Borra la pantalla. */

        k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

    setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
<<<<<<< HEAD
        setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);

/* Carga de IDTR    */

        idtr.base = 0;
        idtr.base +=(dword) &idt;
        idtr.limit = sizeof(idt)-1;
=======
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);

/* Carga de IDTR    */

	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;

	_lidt (&idtr);
>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df

        _lidt (&idtr);

        _Cli();
/* Habilito interrupcion de timer tick*/

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);

<<<<<<< HEAD
        _Sti();
=======
	_Sti();
>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df

        while(1)
        {
        //print("Hola");
        }
<<<<<<< HEAD
=======

}
>>>>>>> c6f659e688919a18b5bab5f6f98638d0168432df

}
