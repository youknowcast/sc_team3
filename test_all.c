#include <stdio.h>

#define IS_NOT_SAFECLIB
#define IS_SLIBC
#define IS_NOT_VC

#ifdef IS_SAFECLIB
#include "../safeclib/include/safe_lib.h"
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
	 バッファ長だけを返すインタフェースもないので、とてつもなく使いにくい。厳密にやるならば、ロックを取って getenv() して、文字数数えて、 malloc() だが、
	 そこまでするならロック取って getenv() して、 dup で十分。
	 なお getenv_s() はバッファ超不足のときは落ちるのではなくて ERANGE を返す模様。
	 */
 
	size_t len;
	char buf[1024];
	int r = getenv_s(&len, buf, sizeof(buf), "PATH");
	printf("%d %d %s\n", r, len, buf);
}

void test_gets() {
	/*
	 みんな大好き gets() は明確に廃止された。
	 gets_s() が利用可能。
	 実装のない向きは、 fgets() が代用可能
	 */
	char buf[16];
	out_log("plz type word: 1234567890");
	gets(buf);
	printf("%s\n", buf);
	gets_s(buf, sizeof(buf));
	printf("%s\n", buf);
	/* fgets を使う場合は末尾が改行ではない場合はバッファが足りなかったはず。*/
	fgets(buf, sizeof(buf), stdin);
}

void test_printf() {
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
	sprintf_s(str, sizeof(str), "1234567890123456");
	out_log("printf");
	printf(str);
}



int main(void) {

#ifdef IS_NOT_VC
	set_constraint_handler_s(my_constraint_handler);	
#endif

	void (*pfunc)();
	int funcs[3] = {
		test_getenv,
		test_gets,
		test_printf
	};
	
	int i;
	for (i=0; i < 4; i++) {
		pfunc = funcs[i];
		pfunc();
	}
	

	return 0;
}
