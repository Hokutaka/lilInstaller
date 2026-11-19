#ifndef LIL_INSTALLER_H
#define LIL_INSTALLER_H

/// @brief インストール処理の結果を表す列挙型
typedef enum
{
    // インストール処理が成功した場合
    LIL_INSTALL_OK,
    // 引数が不正な場合
    LIL_INSTALL_INVALID_ARGUMENT,
    // コピー元のファイルが開けなかった場合
    LIL_INSTALL_SOURCE_ERROR,
    // コピー先のファイルが作成できなかった場合
    LIL_INSTALL_DESTINATION_ERROR,
    // 入出力エラーが発生した場合
    LIL_INSTALL_IO_ERROR
} LilInstallResult;

/// @brief ファイルを指定された場所へコピーする。
/// @param source コピー元のファイルパス。
/// @param destination コピー先のファイルパス。
/// @return 処理結果。
LilInstallResult lil_install_file(
    const char *source,
    const char *destination
);

#endif