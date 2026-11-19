#ifndef LIL_CONFIG_H
#define LIL_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

// インストーラーの設定を保持する構造体
typedef struct
{
    char *title;
    int width;
    int height;
    uint8_t background_rgb[3];
} LilConfig;

// TOML設定ファイルを読み込む
// 成功時はtrue、失敗時はfalse
// outにはゼロ初期化済みの構造体を渡す
bool lil_config_load(const char *path, LilConfig *out);

// 設定データが所有するメモリを解放する
void lil_config_free(LilConfig *config);

#endif