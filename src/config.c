#include "config.h"

#include <tomlc17.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 設定が所有するメモリを解放する
void lil_config_free(LilConfig *config)
{
    if (config == NULL)
    {
        return;
    }

    free(config->title);
    *config = (LilConfig){0};
}

// TOML設定ファイルを読み込む
bool lil_config_load(const char *path, LilConfig *out)
{
    if (path == NULL || out == NULL)
    {
        fprintf(stderr, "設定の読み込み引数が不正です。\n");
        return false;
    }

    // TOMLを解析
    toml_result_t toml = toml_parse_file_ex(path);

    if (!toml.ok)
    {
        fprintf(stderr, "TOMLの解析に失敗しました: %s\n", toml.errmsg);
        toml_free(toml);
        return false;
    }

    // 必要な設定値を取得
    toml_datum_t title = toml_seek(toml.toptab, "window.title");
    toml_datum_t width = toml_seek(toml.toptab, "window.width");
    toml_datum_t height = toml_seek(toml.toptab, "window.height");
    toml_datum_t background = toml_seek(
        toml.toptab,
        "theme.background_color"
    );

    // タイトルの検証
    if (title.type != TOML_STRING || title.u.s == NULL)
    {
        fprintf(stderr, "ウィンドウタイトルの設定が不正です。\n");
        toml_free(toml);
        return false;
    }

    // ウィンドウサイズの検証
    if (width.type != TOML_INT64 ||
        height.type != TOML_INT64 ||
        width.u.int64 <= 0 ||
        height.u.int64 <= 0 ||
        width.u.int64 > INT_MAX ||
        height.u.int64 > INT_MAX)
    {
        fprintf(stderr, "ウィンドウサイズの設定が不正です。\n");
        toml_free(toml);
        return false;
    }

    // 背景色の配列を検証
    if (background.type != TOML_ARRAY ||
        background.u.arr.size != 3)
    {
        fprintf(stderr, "背景色の設定が不正です。\n");
        toml_free(toml);
        return false;
    }

    uint8_t rgb[3];

    // RGBの各要素を検証
    for (int i = 0; i < 3; i++)
    {
        toml_datum_t value = background.u.arr.elem[i];

        if (value.type != TOML_INT64 ||
            value.u.int64 < 0 ||
            value.u.int64 > 255)
        {
            fprintf(stderr, "背景色は0〜255で指定してください。\n");
            toml_free(toml);
            return false;
        }

        rgb[i] = (uint8_t)value.u.int64;
    }

    // TOMLが管理する文字列を自前のメモリへコピー
    size_t title_length = strlen(title.u.s);
    char *title_copy = malloc(title_length + 1);

    if (title_copy == NULL)
    {
        fprintf(stderr, "タイトル用メモリの確保に失敗しました。\n");
        toml_free(toml);
        return false;
    }

    memcpy(title_copy, title.u.s, title_length + 1);

    // 検証済みの設定を保存
    out->title = title_copy;
    out->width = (int)width.u.int64;
    out->height = (int)height.u.int64;

    for (int i = 0; i < 3; i++)
    {
        out->background_rgb[i] = rgb[i];
    }

    // TOMLの解析結果はここで解放できる
    toml_free(toml);

    return true;
}