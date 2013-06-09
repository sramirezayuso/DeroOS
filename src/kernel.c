#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/stdio.h"
#include "../include/kc.h"
#include "../include/shell.h"

DESCR_INT idt[0xA];                     /* IDT de 10 entradas*/
IDTR idtr;                              /* IDTR */

void int_08() {

    //char *video = (char *) 0xb8000;
    //video[tickpos+=2]='!';

}

void int_09(int scancode) {
    decodeScancode(scancode);
}

size_t __write(int fd, const void* buffer, size_t count) {

        size_t cantW = 0;
        unsigned char * b = (unsigned char *) buffer;
        if(count < 0)
                return -1;

        switch(fd) {
                case STDOUT:
                                while(count) {
                                        _writeScreen(b[cantW], (unsigned char *) screenPosition(2));
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
        char* cBuf;
        if(count < 0)
                return -1;

        switch(fd) {

        case STDOUT:
            while(count) {
                char var = _read((unsigned char * ) 0xb8000);
                cBuf = buffer;
                cBuf[cantR] = var;
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

		initializeShell();
        while(1)
        {
			
        }

}
