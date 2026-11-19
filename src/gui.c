#include "gui.h"
#include <SDL3/SDL.h>

// GUIを起動する
int lil_gui_run(const LilConfig *config)
{
    if (config == NULL || config->title == NULL)
    {
        return 1;
    }

    // SDLを初期化
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDLの初期化に失敗しました: %s", SDL_GetError());
        return 1;
    }

    // ウィンドウを生成
    SDL_Window *window = SDL_CreateWindow(
        config->title,
        config->width,
        config->height,
        0
    );

    if (window == NULL)
    {
        SDL_Log("ウィンドウの生成に失敗しました: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // レンダラーを生成
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL)
    {
        SDL_Log("レンダラーの生成に失敗しました: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 背景色を設定して描画
    int result = 0;

    if (!SDL_SetRenderDrawColor(
            renderer,
            config->background_rgb[0],
            config->background_rgb[1],
            config->background_rgb[2],
            255
        ) ||
        !SDL_RenderClear(renderer) ||
        !SDL_RenderPresent(renderer))
    {
        SDL_Log("描画に失敗しました: %s", SDL_GetError());
        result = 1;
    }
    else
    {
        // ウィンドウを閉じるまで待機
        SDL_Event event;
        bool running = true;

        while (running)
        {
            if (!SDL_WaitEvent(&event))
            {
                SDL_Log("イベント取得に失敗しました: %s", SDL_GetError());
                result = 1;
                break;
            }

            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
    }

    // 終了処理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return result;
}