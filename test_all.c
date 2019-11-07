#include <stdio.h>
#include <string.h>

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
