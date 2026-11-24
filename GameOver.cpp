#include "DxLib.h"

#include "GameOver.h"

#include "SceneMgr.h"

static int KeyState_GameOver[256] = { 0 };


void GameOver_Update() {
	GetHitKeyStateAllEx(KeyState_GameOver);
	if (KeyState_GameOver[KEY_INPUT_Z] == 1) {
		SceneMgr_ChangeScene(eScene_Menu);
	}
}

void GameOver_Draw() {
	DrawString(240, 260, "ゲームオーバー画面", GetColor(255, 255, 255));
	DrawString(240, 280, "（Zキーでメニュー画面）", GetColor(255, 255, 255));
}