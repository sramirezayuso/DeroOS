/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);


#endif
