#include "../include/string.h"



/***************************************************************
*strcmp
*
* Compara dos cadenas de caracteres 
* http://clc-wiki.net/wiki/C_standard_library:string.h:strcmp
****************************************************************/
int strcmp(const char* s1, const char* s2) {
    while(*s1 && (*s1==*s2)) {
		s1++;
		s2++;
	}
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

/***************************************************************
*strlen
*
* Compara dos cadenas de caracteres 
* http://clc-wiki.net/wiki/C_standard_library:string.h:strcmp
****************************************************************/
int strlen(const char *s) {
    int i;
    for (i = 0; s[i] != 0; i++) ;
    return i;
}