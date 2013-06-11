#include "../include/stdio.h"
#include "../include/kernel.h"
#include "../include/shell.h"
#include "../include/kasm.h"


#define CMOS_ADDRESS  	0x70
#define CMOS_DATA		0x71
/* Imprime un caracter por pantalla */
void printChar (char c);

/* Imprime una cadena de caracteres por pantalla y devuelve la longitud del mismo */
int print (char* string);

/* Imprime el entero por pantalla y devuelve la longitud del mismo */
int printInt (int num);

unsigned short pciConfigReadWord (unsigned short bus, unsigned short slot,
				unsigned short func, unsigned short offset);
void lspci();

int printf ( const char * format, ... ) {
	va_list ap;
	va_start(ap, format);

	char * charVal;
	char ** stringVal;
	int * intVal;
	int charsPrinted = 0;
	int i;

	for(i=0 ; format[i] != 0 ; i++) {

		if(format[i] == '%') {

			i++;
			switch(format[i]) {

				case 'i':
				case 'd':
						intVal = va_arg(ap);
						charsPrinted += printInt(*intVal);
						break;
				case 'c':
						charVal = va_arg(ap);
						charsPrinted++;
						putc(*charVal);
						break;
				case 's':
						stringVal = va_arg(ap);
						charsPrinted += print(*stringVal);
						break;
				default:
						break;
			}

		} else {

			putc(format[i]);
			charsPrinted++;

		}


	}

	va_end(ap);
	return charsPrinted;
}

int scanf ( const char * format, ... ) {
	return NULL;
}

int getc ( ) {
	return NULL;
}

int putc ( int character ) {
	printKey( (char) character);
	return character;
}

/***************************************************************
*print
* Imprime por pantalla un string
*
*Recibe: String
****************************************************************/

int print (char* string){
    int counter = 0;

    while (string[counter] != '\0'){

		putc(string[counter]);
    	counter++;

    }

	return counter;
}

int printInt (int num){
	int numLong = 0;

    if(num < 0) {
	putc('-');
	num = num * -1;
    }

    if(num < 10) {
	putc('0'+ num);
	return 1;
    }

    numLong = printInt(num/10);
    putc('0'+ (num % 10));
	return numLong++;
}

void lspci()
 {
	int bus, slot;
	unsigned short vendor, device;
	for(bus = 0; bus < 256; bus++){
		for(slot = 0; slot < 32; slot++){
			vendor = pciConfigReadWord(bus,slot,0,0);
			if(vendor != 0xFFFF){
				device = pciConfigReadWord(bus,slot,0,2);
				printf("%d", vendor);
				putc('c');
				printf("%d", device);
			}
		}
	}
}

 unsigned short pciConfigReadWord (unsigned short bus, unsigned short slot,
				unsigned short func, unsigned short offset)
 {
    unsigned long address;
    unsigned long longBus = (unsigned long)bus;
    unsigned long longSlot = (unsigned long)slot;
    unsigned long longFunc = (unsigned long)func;
    unsigned short tmp = 0;

    address = (unsigned long)((longBus << 16) | (longSlot << 11) | (longFunc << 8)
				| (offset & 0xfc) | ((unsigned long)0x80000000));

    _outl(0x0CF8, address);

    tmp = (unsigned short)((_inl(0x0CFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }

 int get_update_in_progress_flag() {
      _outb((unsigned char *)CMOS_ADDRESS, 0x0A);
      return (_inb(CMOS_DATA) & 0x80);
}

unsigned char ans[9];
unsigned char * read_rtc() {
    unsigned char hour, minute, second;
    unsigned char registerB;


    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (get_update_in_progress_flag());
    _outb((unsigned char *) CMOS_ADDRESS, 0x00);
    second = _inb(CMOS_DATA);
    _outb((unsigned char *)CMOS_ADDRESS, 0x02);
    minute = _inb(CMOS_DATA);
    _outb((unsigned char *)CMOS_ADDRESS, 0x04);
    hour = _inb(CMOS_DATA);

    _outb((unsigned char *)CMOS_ADDRESS, 0x0B);
    registerB = _inb(CMOS_DATA);

    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
    }

    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }
	
	ans[0] = (hour/10) + '0';
    ans[1] = (hour%10) + '0';
	ans[2] = ':';
	ans[3] = (minute/10) + '0';
    ans[4] = (minute%10) + '0';
	ans[5] = ':';
	ans[6] = (second/10) + '0';
    ans[7] = (second%10) + '0';
	ans[8] = 0;
	return ans;
}