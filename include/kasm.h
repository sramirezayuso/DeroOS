/*********************************************
kasm.h

************************************************/

#include "defs.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_int_08_hand();      /* Timer tick */
void		_int_09_hand();      /* Keyboard */

void		_write(byte caracter, byte * pos);
char		_read(byte * pos);

void		_debug (void);

void		_outb(unsigned char * to, unsigned short from);
void		_outl(unsigned short to, unsigned long data);

unsigned long	_inl(unsigned short from);
unsigned long	_inb(unsigned short from);

dword       _read_msr(unsigned short from);
