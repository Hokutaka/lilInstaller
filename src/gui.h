#ifndef LIL_GUI_H
#define LIL_GUI_H

#include "config.h"

// 設定に従ってGUIを起動する
// 正常終了時は0、エラー時は0以外を返す
int lil_gui_run(const LilConfig *config);

#endif