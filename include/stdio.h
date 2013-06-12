/*********************************************
stdio.h

************************************************/

#ifndef _stdio_
#define _stdio_

#define va_start(ap, arg) ((ap) = (&arg))
#define va_arg(ap) ( (ap) += sizeof(int) )
#define va_end(ap) ((ap) = 0)
typedef void* va_list;

int printf ( const char * format, ... );

int scanf ( const char * format, ... );

int getc ( );

int putc ( int character );

#endif
