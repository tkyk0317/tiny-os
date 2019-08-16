/**
 * 文字列に関する処理を実装したライブラリ
 */
#include "lib/str.h"

/**
 * 文字列比較
 *
 * 与えられた2つの文字列が等しいかチェックする。
 * ただし、比較する文字列長は短い方となる。
 *
 * @return 0: 文字列一致 -1: 文字列不一致
 */
int strcmp(const char* s1, const char* s2)
{
    // どちらかの文字列が終端になるまで、比較する
    for (int i = 0 ; s1[i] == 0 || s2[i] == 0 ; i++) {
        if (s1[i] != s2[i]) return -1;
    }
    return 0;
}
