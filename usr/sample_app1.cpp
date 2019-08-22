#include <cstdint>
#include "sys/sys.h"

/**
 * サンプルアプリケーション
 * 
 * ビルドしたものをXMODEMを通じて、起動する為に作成
 */
int main()
{
    uint64_t c = 0;
    while(true) {
        write_sys_call("Sample Application 1-1\n");
        c++;
        write_sys_call("Sample Application 1-2\n");
    }
}
