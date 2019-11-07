#include <stdio.h>

#define __is_safeclib__
#ifdef __is_safeclib__

#include "../safeclib/include/safe_lib.h"

#else
#include <string.h>
#endif

int main(void) {
	char str[16];
//	strcpy_s(str, 13, "Hello World!");
//	sprintf(str, "1234567890123456");
	snprintf(str, sizeof(str),"1234567890123456");
	printf(str);
	sprintf_s(str, sizeof(str), "1234567890123456");
	printf(str);
	return 0;
}
