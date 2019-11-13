
#define __is_not_safeclib__
#ifdef __is_safeclib__

#include "../safeclib/include/safe_lib.h"

#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#endif

void test_getenv() {
	/*
	 みんな大好き getenv() は、環境変数へのポインタを返す。
	 プロセスの環境変数自体は書き換え可能なものであるため、ポインタ値そのままでは危険。
	 もちろん、良い子は dup してから保存するが、レースコンディションもあり得る。
	 getenv_s() を利用するとそのあたりの諸々も問題が解決可能
	 しかし、例によってパラメータとして与えるバッファ長コピー対象のデータ長より長くないとエラーになるため、とても使いにくい。
	 なお getenv_s() はバッファ超不足のときは落ちるのではなくて ERANGE を返したうえ、必用なバッファ長は取得できる模様。
	 */

	size_t len=0;
	char buf[1024];
	int r = getenv_s(&len, buf, sizeof(buf), "PATH");
	printf("%d %d %s\n", r, len, buf);
	len = 0;
	char buf2[16];
	r = getenv_s(&len, buf2, sizeof(buf2), "PATH");
	printf("%d %d %s\n", r, len, buf2);
	char* buf3;
	r = getenv_s(&len, NULL, 0, "PATH");
	buf3 = malloc(len);
	r = getenv_s(&len, buf3, len, "PATH");
	printf("%d %d %s\n", r, len, buf3);
}

void test_gets() {
	/*
	 みんな大好き gets() は明確に廃止された。
	 gets_s() が利用可能。
	 実装のない向きは、 fgets() が代用可能
	 */
	char buf[16];
	char buf2[16];
	char* buf4gets =(buf>buf2)?buf2:buf;
	printf("buf: %p buf2: %p\n", buf, buf2);
	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	gets(buf4gets);
	printf("gets : buf : '%s'\n", buf);
	printf("gets : buf2: '%s'\n", buf2);
	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	/* fgets を使う場合は末尾が改行ではない場合はバッファが足りなかったはず。*/
	fgets(buf4gets, sizeof(buf), stdin);
	printf("fgets: buf : '%s'\n", buf);
	printf("fgets: buf2: '%s'\n", buf2);
	gets_s(buf4gets, sizeof(buf));
	printf("%s\n", buf);
}

void test_sprintf() {
	/*
	 みんな大好き printf() 件 sprintf_s
	 sprintf_s はエラーになる。。
	 実装のない向きは、 snprintf() が代用可能
	snprintf はバッファが足りない場合はバッファ分書く、
	 */
	char str[16];
	//	strcpy_s(str, 13, "Hello World!");
	//	sprintf(str, "1234567890123456");
	snprintf(str, sizeof(str), "12345678901234567890");
	printf(str);
	sprintf_s(str, sizeof(str), "12345678901234567890");
	printf(str);
}

void test_printf() {
	int n;
	printf("hogehoge%n\n", &n);
	printf("%d\n", n);
}

int main(void) {
	test_getenv();
	return 0;
}
