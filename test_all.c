#include <stdio.h>

#define __is_safeclib__
#ifdef __is_safeclib__

#include "../safeclib/include/safe_lib.h"

#else
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
	snprintf(str, sizeof(str), "1234567890123456");
	printf(str);
	sprintf_s(str, sizeof(str), "1234567890123456");
	printf(str);
}

int main(void) {
	return 0;
}
