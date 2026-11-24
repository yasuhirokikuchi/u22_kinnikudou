#include "DxLib.h"

#include "SceneMgr.h"


#define ITEM_SPEED 6  // 初期値の4+乱数0~4
#define ITEM_SIZE 20  // 縦×横の正方形(当たり判定)

#define NUM 34
#define KYARA 70
#define SAKE 30



static int KeyState_Tutorial[256] = { 0 };

static int Chara_t, Dbell_t, Beer_t, Tutorial_t;

// マッチョ
static int Chara_x1_t, Chara_y1_t, Chara_x2_t, Chara_y2_t;
// ダンベル
static int Dbell_x1_t, Dbell_y1_t, Dbel_x2_t, Dbell_y2_t;
// ビール
static int Beer_x1_t, Beer_y1_t, Beer_x2_t, Beer_y2_t;

// アイテムの流れる速度
static int Dbell_Speed_t = 0, Beer_Speed_t = 0;
// 当たり判定ズ
static int Chara_w_t, Chara_h_t,
Dbell_w_t, Dbell_h_t, Dbell_hit_t,
Beer_w_t, Beer_h_t, Beer_hit_t;

// アイテムの出現時間
typedef struct Frame_t {
	int Dbell_t = 0;
	int Beer_t = 0;
	int Dbell_Rand_t = 0;
	int Beer_Rand_t = 0;
	
}Frame_t;
Frame_t Frame_t_Count;

// アイテムが端に到達したとき出現時間を乱数で取得するためのあれ
typedef struct Rand_t {
	int Dbell_t = 0;
	int Beer_t = 0;
	
}Rand_t;
Rand_t Rand_t_Flag;

// フォントサイズ
int Fontsize_t;


//初期化
void Tutorial_Initialize() {



	Chara_x1_t = 0;
	Chara_y1_t = 360;
	Chara_x2_t = Chara_x1_t + KYARA;
	Chara_y2_t = Chara_y1_t + KYARA;

	Chara_w_t = 40;
	Chara_h_t = 60;


	Dbell_x1_t = 600;
	Dbell_y1_t = 460;
	Dbel_x2_t = Dbell_x1_t + NUM;
	Dbell_y2_t = Dbell_y1_t + NUM;

	Dbell_w_t = ITEM_SIZE;
	Dbell_h_t = ITEM_SIZE;


	Beer_x1_t = 600;
	Beer_y1_t = 360;
	Beer_x2_t = Beer_x1_t + NUM;
	Beer_y2_t = Beer_y1_t + NUM;

	Beer_w_t = ITEM_SIZE;
	Beer_h_t = ITEM_SIZE;



	// アイテムフラッグ（0が存在していない,１が存在している）
	Dbell_hit_t = 0;
	Beer_hit_t = 0;

	// アイテム出現時間
	Frame_t_Count.Dbell_t = 0;
	Frame_t_Count.Beer_t = 0;


	// アイテムスピード初期値(乱数+4)
	Dbell_Speed_t = GetRand(ITEM_SPEED) + 4;
	Beer_Speed_t = GetRand(ITEM_SPEED) + 4;

	Frame_t_Count.Dbell_Rand_t = GetRand(4);
	Frame_t_Count.Beer_Rand_t = GetRand(4);

	//画像のロード
	Chara_t = LoadGraph("images/mattyo.png");
	Dbell_t = LoadGraph("images/dan.png");
	Beer_t = LoadGraph("images/ben.png");
	Tutorial_t = LoadGraph("images/tutorial.png");

	// フォントサイズ保存
	Fontsize_t = CreateFontToHandle(NULL, 50, 4);




}


	





//更新
void Tutorial_Update() {


	GetHitKeyStateAllEx(KeyState_Tutorial);

	if (KeyState_Tutorial[KEY_INPUT_E] == 1) {
		SceneMgr_ChangeScene(eScene_Game);
	}
	


	


	// マッチョの処理
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) || (CheckHitKey(KEY_INPUT_A) == 1)) Chara_x1_t -= 4, Chara_x2_t -= 4;
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1 || (CheckHitKey(KEY_INPUT_D) == 1)) Chara_x1_t += 6, Chara_x2_t += 6;
	if (CheckHitKey(KEY_INPUT_UP) == 1 || (CheckHitKey(KEY_INPUT_W) == 1)) Chara_y1_t -= 6, Chara_y2_t -= 6;
	if (CheckHitKey(KEY_INPUT_DOWN) == 1 || (CheckHitKey(KEY_INPUT_S) == 1)) Chara_y1_t += 6, Chara_y2_t += 6;

	// 上の上限
	if (Chara_y1_t <= 270 - KYARA) {
		Chara_y1_t = 270 - KYARA;
	}
	if (Chara_y2_t <= 270) {
		Chara_y2_t = Chara_y1_t + KYARA;
	}
	// 左の上限
	if (Chara_x1_t <= 0) {
		Chara_x1_t = 1;
	}
	if (Chara_x2_t <= Chara_x1_t + KYARA) {
		Chara_x2_t = Chara_x1_t + KYARA;
	}
	// 下の上限
	if (Chara_y1_t >= 479 - KYARA) {
		Chara_y1_t = 479 - KYARA;
	}
	if (Chara_y2_t >= Chara_y1_t + KYARA) {
		Chara_y2_t = Chara_y1_t + KYARA;
	}
	// 右の上限
	if (Chara_x1_t >= 520) {
		Chara_x1_t = 519;
	}
	if (Chara_x2_t >= Chara_x1_t + KYARA) {
		Chara_x2_t = Chara_x1_t + KYARA;
	}





	// ダンベルの流れの処理
	if (Dbell_hit_t == 1) {
		Dbell_x1_t = Dbell_x1_t - Dbell_Speed_t;
		Dbel_x2_t = Dbel_x2_t - Dbell_Speed_t;
	}


	// 上の上限
	if (Dbell_y1_t <= 280) {
		Dbell_y1_t = 280;
	}
	if (Dbell_y2_t <= Dbell_y1_t + NUM) {
		Dbell_y2_t = Dbell_y1_t + NUM;
	}

	// 下の上限
	if (Dbell_y1_t >= 479 - NUM) {
		Dbell_y1_t = 479 - NUM;
	}
	if (Dbell_y2_t >= Dbell_y1_t + NUM) {
		Dbell_y2_t = Dbell_y1_t + NUM;
	}
	// 端についた時またはマッチョに当たった場合の処理
	if (((Dbell_x1_t > Chara_x1_t && Dbell_x1_t < Chara_x1_t + Chara_w_t) ||
		(Chara_x1_t > Dbell_x1_t && Chara_x1_t < Dbell_x1_t + Dbell_w_t)) &&
		((Dbell_y1_t > Chara_y1_t && Dbell_y1_t < Chara_y1_t + Chara_h_t) ||
			(Chara_y1_t > Dbell_y1_t && Chara_y1_t < Dbell_y1_t + Dbell_h_t))) {

		if (Dbell_hit_t == 1) {
			Dbell_Speed_t = GetRand(ITEM_SPEED) + 4;
			Frame_t_Count.Dbell_Rand_t = GetRand(4);
			
		}
		Dbell_hit_t = 0;
	}

	if (Dbell_x1_t <= 0 || Dbell_hit_t == 0) {

		Frame_t_Count.Dbell_t++;

		if (Dbell_x1_t <= 0 && Rand_t_Flag.Dbell_t == 0) { // 端についた時にも乱数を獲得する
			Dbell_Speed_t = GetRand(ITEM_SPEED) + 4;
			Frame_t_Count.Dbell_Rand_t = GetRand(4);
			Rand_t_Flag.Dbell_t = 1;
		}


		if (Frame_t_Count.Dbell_t == Frame_t_Count.Dbell_Rand_t * 60 || Frame_t_Count.Dbell_Rand_t == 0) {

			Dbell_x1_t = 620;
			Dbel_x2_t = Dbell_x1_t + NUM;
			Dbell_y1_t = 280 + (GetRand(19) + 1) * 10;
			Dbell_y2_t = Dbell_y1_t + NUM;

			Dbell_hit_t = 1;
			Rand_t_Flag.Dbell_t = 0;
			Frame_t_Count.Dbell_t = 0;
		}

	}





	// ビールの流れの処理
	if (Beer_hit_t == 1) {
		Beer_x1_t = Beer_x1_t - Beer_Speed_t;
		Beer_x2_t = Beer_x2_t - Beer_Speed_t;
	}

	// 上の上限
	if (Beer_y1_t <= 280) {
		Beer_y1_t = 280;
	}
	if (Beer_y2_t <= Beer_y1_t + NUM) {
		Beer_y2_t = Beer_y1_t + NUM;
	}

	// 下の上限
	if (Beer_y1_t >= 479 - NUM) {
		Beer_y1_t = 479 - NUM;
	}
	if (Beer_y2_t >= Beer_y1_t + NUM) {
		Beer_y2_t = Beer_y1_t + NUM;
	}
	// 端についた時またはマッチョに当たった場合の処理
	if (((Beer_x1_t > Chara_x1_t && Beer_x1_t < Chara_x1_t + Chara_w_t) ||
		(Chara_x1_t > Beer_x1_t && Chara_x1_t < Beer_x1_t + Beer_w_t)) &&
		((Beer_y1_t > Chara_y1_t && Beer_y1_t < Chara_y1_t + Chara_h_t) ||
			(Chara_y1_t > Beer_y1_t && Chara_y1_t < Beer_y1_t + Beer_h_t))) {

		if (Beer_hit_t == 1) {
			
			Beer_Speed_t = GetRand(ITEM_SPEED) + 4;
			Frame_t_Count.Beer_Rand_t = GetRand(4);
		}
		Beer_hit_t = 0;
	}

	if (Beer_x1_t <= 0 || Beer_hit_t == 0) {

		Frame_t_Count.Beer_t++;

		if (Beer_x1_t <= 0 && Rand_t_Flag.Beer_t == 0) {
			Beer_Speed_t = GetRand(ITEM_SPEED) + 4;
			Frame_t_Count.Beer_Rand_t = GetRand(4);
			Rand_t_Flag.Beer_t = 1;
		}


		if (Frame_t_Count.Beer_t == Frame_t_Count.Beer_Rand_t * 60 || Frame_t_Count.Beer_Rand_t == 0) {

			Beer_x1_t = 620;
			Beer_x2_t = Beer_x1_t + NUM;
			Beer_y1_t = 280 + (GetRand(19) + 1) * 10;
			Beer_y2_t = Beer_y1_t + NUM;

			Beer_hit_t = 1;
			Rand_t_Flag.Beer_t = 0;
			Frame_t_Count.Beer_t = 0;
		}
	}

}



//描画
void Tutorial_Draw() {

	
	DrawGraph(0, 0, Tutorial_t, FALSE);

	// キャラ表示
	DrawExtendGraph(Chara_x1_t, Chara_y1_t, Chara_x2_t, Chara_y2_t, Chara_t, TRUE);


	// アイテム表示(_hitが1の時のみ表示する)
	if (Dbell_hit_t == 1) {
		DrawExtendGraph(Dbell_x1_t, Dbell_y1_t, Dbel_x2_t, Dbell_y2_t, Dbell_t, TRUE);
	}
	if (Beer_hit_t == 1) {
		DrawExtendGraph(Beer_x1_t, Beer_y1_t, Beer_x2_t, Beer_y2_t, Beer_t, TRUE);
	}

	DrawString(10, 10, "操作チュートリアル", GetColor(255, 255, 255));
	DrawString(9, 10, "操作チュートリアル", GetColor(0, 0, 0));
	
	DrawString(0, 100, "ゲーム開始(Eキー)", GetColor(255, 255, 255));
	DrawString(-1, 100, "ゲーム開始(Eキー)", GetColor(0, 0, 0));
	
	DrawString(80, 40, "WASD（↑←↓→）でキャラクターを動かしてね！", GetColor(255, 255, 255));
	DrawString(79, 40, "WASD（↑←↓→）でキャラクターを動かしてね！", GetColor(0, 0, 0));
	
	DrawString(80, 70, "飛んでくるお酒をよけながらハイスコアを目指そう！", GetColor(255, 255, 255));
	DrawString(79, 70, "飛んでくるお酒をよけながらハイスコアを目指そう！", GetColor(0, 0, 0));

}

void Tutorial_Finalize() {
	DeleteGraph(Chara_t);
	DeleteGraph(Dbell_t);
	DeleteGraph(Beer_t);
	DeleteGraph(Tutorial_t);
}


