/*********************************************
stdio.h

************************************************/

#ifndef _stdio_
#define _stdio_


#define NULL 0
#define va_start(ap, arg) ((ap) = (&arg))
#define va_arg(ap) ( (ap) += sizeof(int) )
#define va_end(ap) ((ap) = NULL)
typedef void* va_list;


int printf ( const char * format, ... );

int scanf ( const char * format, ... );

int getc ( /*FILE * stream*/ );

int putc ( int character/*, FILE * stream */);

int putchar ( int character );

int getchar ( void );


#endif