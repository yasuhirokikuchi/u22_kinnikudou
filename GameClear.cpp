#include "DxLib.h"

#include "GameClear.h"

#include "SceneMgr.h"


static int KeyState_GameClear[256] = { 0 };

static int Clear;


//初期化
void GameClear_Initialize() {
	Clear = LoadGraph("images/clearpect.png");
	
}

//終了処理
void GameClear_Finalize() {
	DeleteGraph(Clear);
}


void GameClear_Update() {
	GetHitKeyStateAllEx(KeyState_GameClear);
	if (KeyState_GameClear[KEY_INPUT_Z] == 1) {
		SceneMgr_ChangeScene(eScene_Menu);
	}
}

void GameClear_Draw() {
	
	DrawGraph(0, 0, Clear, FALSE);
	DrawString(280, 240, "クリアおめでとう！", GetColor(255, 255, 255));
	DrawString(280, 260, "てめーは最強のマッチョだ！", GetColor(255, 255, 255));
	DrawString(280, 280, "（Zキーでメニュー画面）", GetColor(255, 255, 255));

}