@echo off
setlocal

rem UTF-8を使用
chcp 65001 >nul

rem リポジトリのルートへ移動
cd /d "%~dp0.."

rem ビルドディレクトリを固定
set "BUILD_DIR=%CD%\build"

rem Visual Studio 2022 Build Tools
set "VSDEVCMD=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"

rem ビルド種別を指定（デフォルトはDebug）
set "BUILD_TYPE=Debug"
if /I "%~1"=="release" (
    set "BUILD_TYPE=Release"
)

echo ビルド構成: %BUILD_TYPE%

rem MSVC x64環境を初期化
if not exist "%VSDEVCMD%" (
    echo Visual Studioの開発者環境が見つかりません。
    exit /b 1
)

call "%VSDEVCMD%" -arch=amd64 -host_arch=amd64
if errorlevel 1 exit /b 1

rem コードページを確認
chcp

rem UTF-8に再設定
chcp 65001 >nul

where cl >nul 2>&1
if errorlevel 1 (
    echo エラー: MSVCコンパイラが見つかりません。
    exit /b 1
)

if /I not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo エラー: x64開発環境ではありません。
    exit /b 1
)

rem CMakeの構成
echo CMakeを構成しています...
cmake -S . -B "%BUILD_DIR%" -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 exit /b 1

rem ビルド
echo ビルドを開始します...
cmake --build "%BUILD_DIR%"
if errorlevel 1 exit /b 1

rem LilInstallerを起動
echo ビルドが正常に完了しました。LilInstallerを起動します...
"%BUILD_DIR%\lilinstaller.exe"

exit /b %errorlevel%