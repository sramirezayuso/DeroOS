/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

unsigned short pciConfigReadWord (unsigned short bus, unsigned short slot,
				unsigned short func, unsigned short offset);
				
void lspci();

unsigned char * read_rtc();

unsigned char * read_temp();

void raise_temp();


#endif
