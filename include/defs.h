/***************************************************
  Defs.h

****************************************************/

#ifndef _defs_
#define _defs_

#define byte unsigned char
#define word short int
#define dword int

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#define CMOS_ADDRESS  	0x70
#define CMOS_DATA		0x71
#define PCI_ADDRESS     0x0CF8
#define PCI_DATA        0x0CFC
#define REGISTER_A      0X0A
#define REGISTER_B		0x0B
#define HOUR_REG		0x04
#define MINUTE_REG		0x02
#define SECOND_REG		0x00
#define MSR_IA32_THERM_STATUS 0x19C
#define MSR_IA32_TEMPERATURE_TARGET 0x1A2

#define STDIN			0
#define STDOUT			1
#define ROWS            25
#define COLS            80
#define NULL			0

#define FALSE			0
#define TRUE			!FALSE

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;

unsigned char screen[ROWS][COLS]; /* screen[filas][columnas]*/
int promptLength;

#endif

