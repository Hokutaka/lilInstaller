# CMakeLists.txt
cmake_minimum_required(VERSION 3.21)

# プロジェクトの名前と使用する言語を指定
project(LilInstaller LANGUAGES C)

# Cの標準を設定
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ビルドタイプを指定（デフォルトはDebug）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ビルドタイプに応じたコンパイルオプションを設定
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()

# 実行可能ファイルを作成
add_executable(lilinstaller
    src/main.c
    src/config.c
    src/gui.c
)

# SDL3をソースから取得してビルドする
include(FetchContent)

# SDL3のビルド設定を変更する
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

# git cloneでSDL3を取得する
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.20
    GIT_SHALLOW TRUE
)

# SDL3をビルドする
FetchContent_MakeAvailable(SDL3)

# LilInsとSDL3をリンクする
target_link_libraries(lilinstaller PRIVATE SDL3::SDL3)

# TOMLパーサーの取得
FetchContent_Declare(
    tomlc17
    GIT_REPOSITORY https://github.com/cktan/tomlc17.git
    GIT_TAG R261116
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(tomlc17)

# TOMLパーサーを静的ライブラリとしてビルド
add_library(tomlc17 STATIC
    ${tomlc17_SOURCE_DIR}/src/tomlc17.c
)

target_include_directories(tomlc17 PUBLIC
    ${tomlc17_SOURCE_DIR}/src
)

# LilInstallerにリンク
target_link_libraries(lilinstaller PRIVATE tomlc17)

# MSVCでUTF-8のソースコードを正しく処理する
if(MSVC)
    target_compile_options(lilinstaller PRIVATE /utf-8)
    target_compile_options(tomlc17 PRIVATE /utf-8)
endif()