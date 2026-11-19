#include "installer.h"

#include <stdio.h>

/// @brief ファイルを指定された場所へコピーする。
/// @param source コピー元のファイルパス。
/// @param destination コピー先のファイルパス。
/// @return 処理結果。
LilInstallResult lil_install_file(
    const char *source,
    const char *destination
)
{
    // 引数を検証
    if (source == NULL || destination == NULL ||
        source[0] == '\0' || destination[0] == '\0')
    {
        return LIL_INSTALL_INVALID_ARGUMENT;
    }

    // コピー元を読み取り専用で開く
    FILE *src = fopen(source, "rb");

    if (src == NULL)
    {
        return LIL_INSTALL_SOURCE_ERROR;
    }

    // コピー先を新規作成する（既存ファイルは上書きしない）
    FILE *dst = fopen(destination, "wbx");

    if (dst == NULL)
    {
        fclose(src);
        return LIL_INSTALL_DESTINATION_ERROR;
    }

    unsigned char buffer[8192];
    LilInstallResult result = LIL_INSTALL_OK;

    // ファイルを分割して読み書きする
    while (1)
    {
        size_t bytes_read = fread(
            buffer, 1, sizeof(buffer), src
        );

        if (bytes_read > 0)
        {
            size_t bytes_written = fwrite(
                buffer, 1, bytes_read, dst
            );

            if (bytes_written != bytes_read)
            {
                result = LIL_INSTALL_IO_ERROR;
                break;
            }
        }

        if (bytes_read < sizeof(buffer))
        {
            if (ferror(src))
            {
                result = LIL_INSTALL_IO_ERROR;
            }

            break;
        }
    }

    // 書き込みの最終結果を確認
    if (fclose(dst) != 0)
    {
        result = LIL_INSTALL_IO_ERROR;
    }

    if (fclose(src) != 0)
    {
        result = LIL_INSTALL_IO_ERROR;
    }

    // 失敗時は今回作成したファイルの削除を試みる
    if (result != LIL_INSTALL_OK)
    {
        remove(destination);
    }

    return result;
}