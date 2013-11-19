#include "../include/stdio.h"
#include "../include/kernel.h"
#include "../include/shell.h"
#include "../include/kasm.h"

/* Imprime una cadena de caracteres por pantalla y devuelve la longitud del mismo */
int print (char* string);

/* Imprime el entero por pantalla y devuelve la longitud del mismo */
int printInt (int num);

/* Devuelve un entero a partir de la posicion dada por el cursor */
int getInt();

/* Devuelve un string a partir de la posicion dada por el cursor */
void getString(char * s);

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
	va_list ap;
	va_start(ap, format);

	char ** charVal;
	char *** stringVal;
	int ** intVal;
	int valReaded = 0;
	int i;

	for(i=0 ; format[i] != 0 ; i++) {

		if(format[i] == '%') {

			i++;
			switch(format[i]) {

				case 'i':
				case 'd':
						intVal = va_arg(ap);
						**intVal = getInt();
						valReaded++;
						break;
				case 'c':
						charVal = va_arg(ap);
						**charVal = getc();
						valReaded++;
						break;
				case 's':
						stringVal = va_arg(ap);
						getString(*stringVal);
						valReaded++;
						break;
				default:
						break;
			}

		}

	}

	va_end(ap);
	return valReaded;
}

int getc ( ) {
	int c;

	__read(STDIN, &c, 1);
	return c;
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
    int i = 0;

    while (string[i] != 0){

		putc(string[i]);
    	i++;

    }

	return i;
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

int getInt() {
	int ret;
	char c;
	
	ret = 0;
	while( (c=getc()) != 0) {
		ret *= 10;
		ret += c - '0';
	}

	return ret;
}


void getString(char * s) {
	char c;
	int i=0;
	
	while( (c=getc()) != 0 && i < BUFF) {
		s[i++] = c;
	}
	s[i] = 0;
}