#include "../include/stdio.h"



int printf ( const char * format, ... ) {
return 0;
}

int scanf ( const char * format, ... ) {
return 0;
}

int getc ( /*FILE * stream */) {
return 0;
}

int putc ( int character/*, FILE * stream*/ ) {
return 0;
}

int putchar ( int character ) {
	__write(1, &character, 1);
	return character;
}

int getchar ( void ) {
return 0;
}





/***************************************************************
*print
* Imprime por pantalla un string
*
*Recibe: String
****************************************************************/
void printChar (char c) {
	__write(1, &c, 1);	
}

void print (char* string){
    int counter = 0;

    while (string[counter] != '\0'){
    	
		printChar(string[counter]);
    	counter++;

    }
   
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