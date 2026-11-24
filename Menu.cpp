#include "DxLib.h"

#include "Menu.h"

#include "SceneMgr.h"


static int KeyState_Menu[256] = { 0 };

static int Fire, logo;    //画像ハンドル格納用変数

//初期化
void Menu_Initialize() {

	Fire = LoadGraph("images/fire.ogv");    //画像のロード
	logo = LoadGraph("images/logo1.png");

	PlayMovieToGraph(Fire);
}

//終了処理
void Menu_Finalize() {
	DeleteGraph(Fire);    //画像の解放
	DeleteGraph(logo);
}

//更新
void Menu_Update() {

	GetHitKeyStateAllEx(KeyState_Menu);

	if (KeyState_Menu[KEY_INPUT_Z] == 1) {//Zキーが押されていたら
		SceneMgr_ChangeScene(eScene_Tutorial);//シーンをゲーム画面に変更
	}
	if (KeyState_Menu[KEY_INPUT_X] == 1) {//Xキーが押されていたら
		SceneMgr_ChangeScene(eScene_Game_endless);//シーンをゲーム（エンドレス）画面に変更
	}
	if (KeyState_Menu[KEY_INPUT_ESCAPE] == 1) {//Escキーが押されていたら
		DxLib_End();
	}
}

//描画
void Menu_Draw() {
	
	if (GetMovieStateToGraph(Fire) == 0) {

		Fire = LoadGraph("images/fire.ogv");
		PlayMovieToGraph(Fire);

	}
	DrawGraph(0, 0, Fire, FALSE);
	DrawGraph(200, 50, logo, TRUE);

    DrawString(240, 230, "ゲーム開始(Zキー)", GetColor(255, 255, 255));
	DrawString(240, 260, "エンドレスモード(Xキー)", GetColor(255, 255, 255));
	DrawString(240, 290, "ゲーム終了(Escキー)", GetColor(255, 255, 255));

}

