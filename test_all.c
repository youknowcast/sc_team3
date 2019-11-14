#include <stdio.h>
#include <stdint.h>

#define IS_NOT_SAFECLIB
#define IS_NOT_SLIBC

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#else
#define IS_NOT_VC
#endif

#ifdef IS_SAFECLIB
#include "./safeclib/include/safe_lib.h"
#include "./safeclib/include/safe_str_lib.h"
#endif

#ifdef IS_SLIBC
#include "./slibc-0.9.2/include/slibc/string.h"
#include <stdlib.h>
#endif

#ifndef IS_SAFECLIB
#ifndef IS_SLIBC
#include <string.h>
#include <stdlib.h>
#endif
#endif

#ifdef IS_NOT_VC
static void my_constraint_handler(const char* msg, void* ptr, errno_t error)
{
    fputs("実行時制約違反が発生: ", stderr);
    fputs(msg, stderr);
    fprintf(stderr, "error code: %d\n", error);
}
#endif

void out_log(char* msg) {
	printf("\n\n[TEST] %s\n", msg);
}

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

	out_log("getenv_s");
	int r = getenv_s(&len, buf, sizeof(buf), "PATH");
	printf("ret: %d length: %d buf: %s\n", r, len, buf);
	len = 0;
	char buf2[16];

	out_log("getenv_s buf length is small.");
	r = getenv_s(&len, buf2, sizeof(buf2), "PATH");
	printf("ret: %d length: %d buf: %s\n", r, len, buf2);
	char* buf3;
	out_log("getenv_s buf length is NULL or 0.");
#ifdef IS_SAFECLIB
	r = getenv_s(&len, buf, 1, "PATH");
#else
	r = getenv_s(&len, NULL, 0, "PATH");
#endif
	buf3 = malloc(len);
	r = getenv_s(&len, buf3, len, "PATH");
	printf("ret: %d length: %d buf: %s\n", r, len, buf3);
}

void test_gets() {
	/*
	 みんな大好き gets() は明確に廃止された。
	 gets_s() が利用可能。
	 実装のない向きは、 fgets() が代用可能
	 */
	char buf[16];
	out_log("plz type word: 1234567890");
	char buf2[16];
	char* buf4gets =(buf>buf2)?buf2:buf;

	out_log("buf information");
	printf("buf: %p buf2: %p\n", buf, buf2);

	out_log("gets");
	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	gets(buf4gets);
	printf("gets : buf : '%s'\n", buf);
	printf("gets : buf2: '%s'\n", buf2);
	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));

	/* fgets を使う場合は末尾が改行ではない場合はバッファが足りなかったはず。*/
	out_log("fgets");
	fgets(buf4gets, sizeof(buf), stdin);
	printf("fgets: buf : '%s'\n", buf);
	printf("fgets: buf2: '%s'\n", buf2);

	out_log("gets_s");
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
	out_log("snprintf");
	snprintf(str, sizeof(str), "1234567890123456");
	out_log("printf");
	printf(str);
	out_log("sprintf_s");
//	sprintf_s(str, sizeof(str), "1234567890123456");
	out_log("printf");
	printf(str);
}

void test_printf() {
	int n;
	out_log("printf with \%n");
	printf("hogehoge%n\n", &n);
	printf("%d\n", n);

#ifdef IS_SLIBC
	out_log("printf_s with \%n");
	printf_s("hogehoge%n\n", &n);
	printf_s("%d\n", n);
#endif
}

int main(void) {
#ifdef IS_NOT_VC
  set_constraint_handler_s(my_constraint_handler);
  printf("not vc\n");
#endif

#ifdef IS_SAFECLIB
    // なぜか関数ポインタ呼び出しで segv
    test_getenv();
    test_gets();
    test_sprintf();
    test_printf();
#else
	void (*pfunc)();
	int funcs[4] = {
		(intptr_t)test_getenv,
		(intptr_t)test_gets,
		(intptr_t)test_sprintf,
		(intptr_t)test_printf
	};
	
	int i;
	for (i=0; i < 4; i++) {
		pfunc = (intptr_t)funcs[i];
		pfunc();
  }
#endif
}


