/**
 * 文字列関連のライブラリヘッダー
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// 文字列比較関数
int strcmp(const char*, const char*);
int strncmp(const char*, const char*, int);

#ifdef __cplusplus
}
#endif
