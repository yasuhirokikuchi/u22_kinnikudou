#include "DxLib.h"

#include "Game_endless.h"

#include "SceneMgr.h"


#define ITEM_DBELL 1 // アイテムスコアいじるとこ
#define ITEM_NIKU 1

#define ITEM_SPEED 6  // 初期値の4+乱数0~4
#define ITEM_SIZE 20  // 縦×横の正方形(当たり判定)

#define NUM 34
#define KYARA 70
#define SAKE 30

static int Dbell, Beer, Sake, Niku, Chara_Max, Chara_Min;

static int Screen_sky, Screen_road;
static int Sky1, Sky2, Sky3, Road1, Road2, Road3;

// マッチョ
static int Chara_x1, Chara_y1, Chara_x2, Chara_y2;
// マッチョの状態変化
static int mattyo_Speed = 0, mattyo_state;
// 効果音
static int Sound_Effect, Sound_Effect_Down, Sound_Effect_Up, Sound_Count = 2;
// ダンベル
static int Dbell_x1, Dbell_y1, Dbel_x2, Dbell_y2;
// ビール
static int Beer_x1, Beer_y1, Beer_x2, Beer_y2;
// 日本酒
static int Sake_x1, Sake_y1, Sake_x2, Sake_y2;
// 肉
static int Niku_x1, Niku_y1, Niku_x2, Niku_y2;

// アイテムの流れる速度
static int Dbell_Speed = 0, Beer_Speed = 0, Sake_Speed = 0, Niku_Speed = 0;

// 当たり判定ズ
static int Chara_w, Chara_h,
Dbell_w, Dbell_h, Dbell_hit,
Beer_w, Beer_h, Beer_hit,
Sake_w, Sake_h, Sake_hit,
Niku_w, Niku_h, Niku_hit;

// アイテムの出現時間
typedef struct Frame {
	int Dbell = 0;
	int Beer = 0;
	int Sake = 0;
	int Niku = 0;
	int Pause = 0;
	int Wait = 0;
	int Dbell_Rand = 0;
	int Beer_Rand = 0;
	int Sake_Rand = 0;
	int Niku_Rand = 0;
}Frame;
static Frame Frame_Count;

// アイテムが端に到達したとき出現時間を乱数で取得するためのあれ
typedef struct Rand {
	int Dbell = 0;
	int Beer = 0;
	int Sake = 0;
	int Niku = 0;
}Rand;
static Rand Rand_Flag;

// スコア
static int Score = 0;

// BGM
static int SHandle;

// キー入力
static int KeyState_Game[256] = { 0 };

// 残り時間
static int Time_count = 0;

// スタート時間
static int Start, Start_flag, Start_Draw;

// フォントサイズ
static int Fontsize;

// デバッグスイッチ
static bool debug = true;

static int End_flag = 0;

void Game_endless_Initialize() {

	Start_flag = 0;
	Start_Draw = 0;
	Start = 0;

	Time_count = 0;

	Score = 0;


	Sky1 = 0;
	Sky2 = 639;
	Sky3 = 639;
	Road1 = 0;
	Road2 = 639;
	Road3 = 639;


	Chara_x1 = 0;
	Chara_y1 = 360;
	Chara_x2 = Chara_x1 + KYARA;
	Chara_y2 = Chara_y1 + KYARA;

	Chara_w = 40;
	Chara_h = 60;


	mattyo_state = 2;

	Dbell_x1 = 600;
	Dbell_y1 = 460;
	Dbel_x2 = Dbell_x1 + NUM;
	Dbell_y2 = Dbell_y1 + NUM;

	Dbell_w = ITEM_SIZE;
	Dbell_h = ITEM_SIZE;


	Beer_x1 = 600;
	Beer_y1 = 360;
	Beer_x2 = Beer_x1 + NUM;
	Beer_y2 = Beer_y1 + NUM;

	Beer_w = ITEM_SIZE;
	Beer_h = ITEM_SIZE;


	Sake_x1 = 600;
	Sake_y1 = 200;
	Sake_x2 = Sake_x1 + SAKE;
	Sake_y2 = Sake_y1 + SAKE;

	Sake_w = ITEM_SIZE;
	Sake_h = ITEM_SIZE;


	Niku_x1 = 600;
	Niku_y1 = 200;
	Niku_x2 = Niku_x1 + SAKE;
	Niku_y2 = Niku_y1 + SAKE;

	Niku_w = ITEM_SIZE;
	Niku_h = ITEM_SIZE;

	// アイテムフラッグ（0が存在していない,１が存在している）
	Dbell_hit = 0;
	Beer_hit = 0;
	Sake_hit = 0;
	Niku_hit = 0;

	// アイテム出現時間
	Frame_Count.Dbell = 0;
	Frame_Count.Beer = 0;
	Frame_Count.Sake = 0;
	Frame_Count.Niku = 0;


	// アイテムスピード初期値(乱数+4)
	Dbell_Speed = GetRand(ITEM_SPEED) + 4;
	Beer_Speed = GetRand(ITEM_SPEED) + 4;
	Sake_Speed = GetRand(ITEM_SPEED) + 4;
	Niku_Speed = GetRand(ITEM_SPEED) + 4;

	Frame_Count.Dbell_Rand = GetRand(4);
	Frame_Count.Beer_Rand = GetRand(4);
	Frame_Count.Sake_Rand = GetRand(4);
	Frame_Count.Niku_Rand = GetRand(4);

	//画像のロード
	Chara_Max = LoadGraph("images/mattyoMax.png");
	Chara_Min = LoadGraph("images/mattyoMin.png");
	Screen_sky = LoadGraph("images/sky1.jpg");
	Screen_road = LoadGraph("images/road1.jpg");
	Dbell = LoadGraph("images/dan.png");
	Beer = LoadGraph("images/ben.png");
	Sake = LoadGraph("images/bottle.png");
	Niku = LoadGraph("images/niku.png");

	// フォントサイズ保存
	Fontsize = CreateFontToHandle(NULL, 50, 4);

	// BGM保存
	SHandle = LoadSoundMem("muscle2.mp3");
	Sound_Effect = LoadSoundMem("Item_Get.wav");
	Sound_Effect_Down = LoadSoundMem("mattyo_Down.wav");
	Sound_Effect_Up = LoadSoundMem("mattyo_Up.wav");

	// 音量の設定
	ChangeVolumeSoundMem(255 * 50 / 100, SHandle);
	ChangeVolumeSoundMem(255 * 50 / 100, SHandle);
	ChangeVolumeSoundMem(255 * 30 / 100, Sound_Effect);
	ChangeVolumeSoundMem(255 * 30 / 100, Sound_Effect_Down);
	ChangeVolumeSoundMem(255 * 40 / 100, Sound_Effect_Up);

	// 音の再生
	PlaySoundMem(SHandle, DX_PLAYTYPE_BACK);
}



void Game_endless_Finalize() {
	//画像の解放
	DeleteGraph(Screen_sky);
	DeleteGraph(Screen_road);
	DeleteGraph(Dbell);
	DeleteGraph(Beer);
	DeleteGraph(Sake);
	DeleteGraph(Niku);
	InitSoundMem();
	Sound_Count = 2;
}



void Game_endless_Update() {

	if (Start_flag == 0) {
		Start++;
	}

	if (Start == 60 * 1) {
		Start_Draw++;
	}
	if (Start == 60 * 2) {
		Start_Draw++;
	}
	if (Start == 60 * 3) {
		Start_Draw++;
	}
	if (Start == 60 * 4) {
		Start_Draw++;
	}

	GetHitKeyStateAllEx(KeyState_Game);


	if (KeyState_Game[KEY_INPUT_0] == 1) {// デバッグモード切り替え
		if (debug == true) {
			debug = false;
		}
		else {
			debug = true;
		}
	}




	// 背景処理
	Sky1 -= 2;
	Sky2 -= 2;
	Sky3 -= 2;

	Road1 -= 5;
	Road2 -= 5;
	Road3 -= 5;

	if (Sky1 <= 0) {
		Sky3 = 639;
	}
	if (Sky2 <= 0) {
		Sky1 = 639;
	}
	if (Sky3 <= 0) {
		Sky2 = 639;
	}

	if (Road1 <= 0) {
		Road3 = 639;
	}
	if (Road2 <= 0) {
		Road1 = 639;
	}
	if (Road3 <= 0) {
		Road2 = 639;
	}

	if (Start >= 60 * 3) {
		if (Start >= 60 * 4) {
			Start_flag = 1;
		}
		// 残り時間処理
		Time_count++;

		// マッチョの処理
		if ((CheckHitKey(KEY_INPUT_LEFT) == 1) || (CheckHitKey(KEY_INPUT_A) == 1)) Chara_x1 -= 4 + (mattyo_Speed), Chara_x2 -= 4 + (mattyo_Speed);
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1 || (CheckHitKey(KEY_INPUT_D) == 1)) Chara_x1 += 6 + (mattyo_Speed), Chara_x2 += 6 + (mattyo_Speed);
		if (CheckHitKey(KEY_INPUT_UP) == 1 || (CheckHitKey(KEY_INPUT_W) == 1)) Chara_y1 -= 6 + (mattyo_Speed), Chara_y2 -= 6 + (mattyo_Speed);
		if (CheckHitKey(KEY_INPUT_DOWN) == 1 || (CheckHitKey(KEY_INPUT_S) == 1)) Chara_y1 += 6 + (mattyo_Speed), Chara_y2 += 6 + (mattyo_Speed);

		// 上の上限
		if (Chara_y1 <= 270 - KYARA) {
			Chara_y1 = 270 - KYARA;
		}
		if (Chara_y2 <= 270) {
			Chara_y2 = Chara_y1 + KYARA;
		}
		// 左の上限
		if (Chara_x1 <= 0) {
			Chara_x1 = 1;
		}
		if (Chara_x2 <= Chara_x1 + KYARA) {
			Chara_x2 = Chara_x1 + KYARA;
		}
		// 下の上限
		if (Chara_y1 >= 479 - KYARA) {
			Chara_y1 = 479 - KYARA;
		}
		if (Chara_y2 >= Chara_y1 + KYARA) {
			Chara_y2 = Chara_y1 + KYARA;
		}
		// 右の上限
		if (Chara_x1 >= 520) {
			Chara_x1 = 519;
		}
		if (Chara_x2 >= Chara_x1 + KYARA) {
			Chara_x2 = Chara_x1 + KYARA;
		}





		// ダンベルの流れの処理
		if (Dbell_hit == 1) {
			Dbell_x1 = Dbell_x1 - Dbell_Speed;
			Dbel_x2 = Dbel_x2 - Dbell_Speed;
		}


		// 上の上限
		if (Dbell_y1 <= 280) {
			Dbell_y1 = 280;
		}
		if (Dbell_y2 <= Dbell_y1 + NUM) {
			Dbell_y2 = Dbell_y1 + NUM;
		}

		// 下の上限
		if (Dbell_y1 >= 479 - NUM) {
			Dbell_y1 = 479 - NUM;
		}
		if (Dbell_y2 >= Dbell_y1 + NUM) {
			Dbell_y2 = Dbell_y1 + NUM;
		}
		// 端についた時またはマッチョに当たった場合の処理
		if (((Dbell_x1 > Chara_x1 && Dbell_x1 < Chara_x1 + Chara_w) ||
			(Chara_x1 > Dbell_x1 && Chara_x1 < Dbell_x1 + Dbell_w)) &&
			((Dbell_y1 > Chara_y1 && Dbell_y1 < Chara_y1 + Chara_h) ||
				(Chara_y1 > Dbell_y1 && Chara_y1 < Dbell_y1 + Dbell_h))) {

			if (Dbell_hit == 1) {
				Dbell_Speed = GetRand(ITEM_SPEED) + 4;
				Frame_Count.Dbell_Rand = GetRand(4);
				Score += ITEM_DBELL;
			}
			Dbell_hit = 0;
		}

		if (Dbell_x1 <= 0 || Dbell_hit == 0) {

			Frame_Count.Dbell++;

			if (Dbell_x1 <= 0 && Rand_Flag.Dbell == 0) { // 端についた時にも乱数を獲得する
				Dbell_Speed = GetRand(ITEM_SPEED) + 4;
				Frame_Count.Dbell_Rand = GetRand(4);
				Rand_Flag.Dbell = 1;
			}


			if (Frame_Count.Dbell == Frame_Count.Dbell_Rand * 60 || Frame_Count.Dbell_Rand == 0) {

				Dbell_x1 = 620;
				Dbel_x2 = Dbell_x1 + NUM;
				Dbell_y1 = 280 + (GetRand(19) + 1) * 10;
				Dbell_y2 = Dbell_y1 + NUM;

				Dbell_hit = 1;
				Rand_Flag.Dbell = 0;
				Frame_Count.Dbell = 0;
			}

		}





		// ビールの流れの処理
		if (Beer_hit == 1) {
			Beer_x1 = Beer_x1 - Beer_Speed;
			Beer_x2 = Beer_x2 - Beer_Speed;
		}

		// 上の上限
		if (Beer_y1 <= 280) {
			Beer_y1 = 280;
		}
		if (Beer_y2 <= Beer_y1 + NUM) {
			Beer_y2 = Beer_y1 + NUM;
		}

		// 下の上限
		if (Beer_y1 >= 479 - NUM) {
			Beer_y1 = 479 - NUM;
		}
		if (Beer_y2 >= Beer_y1 + NUM) {
			Beer_y2 = Beer_y1 + NUM;
		}

		if (Beer_x1 <= 0 || Beer_hit == 0) {

			Frame_Count.Beer++;

			if (Beer_x1 <= 0 && Rand_Flag.Beer == 0) {
				Beer_Speed = GetRand(ITEM_SPEED) + 4;
				Frame_Count.Beer_Rand = GetRand(4);
				Rand_Flag.Beer = 1;
			}


			if (Frame_Count.Beer == Frame_Count.Beer_Rand * 60 || Frame_Count.Beer_Rand == 0) {

				Beer_x1 = 620;
				Beer_x2 = Beer_x1 + NUM;
				Beer_y1 = 280 + (GetRand(19) + 1) * 10;
				Beer_y2 = Beer_y1 + NUM;

				Beer_hit = 1;
				Rand_Flag.Beer = 0;
				Frame_Count.Beer = 0;
			}
		}




		// 日本酒の流れの処理
		if (Sake_hit == 1) {
			Sake_x1 = Sake_x1 - Sake_Speed;
			Sake_x2 = Sake_x2 - Sake_Speed;
		}

		// 上の上限
		if (Sake_y1 <= 280) {
			Sake_y1 = 280;
		}
		if (Sake_y2 <= Sake_y1 + SAKE) {
			Sake_y2 = Sake_y1 + SAKE;
		}

		// 下の上限
		if (Sake_y1 >= 479 - SAKE) {
			Sake_y1 = 479 - SAKE;
		}
		if (Sake_y2 >= Sake_y1 + SAKE) {
			Sake_y2 = Sake_y1 + SAKE;
		}

		if (Sake_x1 <= 0 || Sake_hit == 0) {

			Frame_Count.Sake++;

			if (Beer_x1 <= 0 && Rand_Flag.Sake == 0) {
				Beer_Speed = GetRand(ITEM_SPEED) + 4;
				Frame_Count.Sake_Rand = GetRand(4);
				Rand_Flag.Sake = 1;
			}

			if (Frame_Count.Sake == Frame_Count.Sake_Rand * 60 || Frame_Count.Sake_Rand == 0) {

				Sake_x1 = 620;
				Sake_x2 = Sake_x1 + NUM;
				Sake_y1 = 280 + (GetRand(19) + 1) * 10;
				Sake_y2 = Sake_y1 + NUM;

				Sake_hit = 1;
				Rand_Flag.Sake = 0;
				Frame_Count.Sake = 0;
			}
		}





		// 肉の流れの処理
		if (Niku_hit == 1) {
			Niku_x1 = Niku_x1 - Niku_Speed;
			Niku_x2 = Niku_x2 - Niku_Speed;
		}

		// 上の上限
		if (Niku_y1 <= 280) {
			Niku_y1 = 280;
		}
		if (Niku_y2 <= Niku_y1 + SAKE) {
			Niku_y2 = Niku_y1 + SAKE;
		}

		// 下の上限
		if (Niku_y1 >= 479 - SAKE) {
			Niku_y1 = 479 - SAKE;
		}
		if (Niku_y2 >= Niku_y1 + SAKE) {
			Niku_y2 = Niku_y1 + SAKE;
		}
		// 端についた時またはマッチョに当たった場合の処理
		if (((Niku_x1 > Chara_x1 && Niku_x1 < Chara_x1 + Chara_w) ||
			(Chara_x1 > Niku_x1 && Chara_x1 < Niku_x1 + Niku_w)) &&
			((Niku_y1 > Chara_y1 && Niku_y1 < Chara_y1 + Chara_h) ||
				(Chara_y1 > Niku_y1 && Chara_y1 < Niku_y1 + Niku_h))) {

			if (Niku_hit == 1) {
				Score += ITEM_NIKU;
				Niku_Speed = GetRand(ITEM_SPEED) + 4;
				Frame_Count.Niku_Rand = GetRand(4);
			}
			Niku_hit = 0;
		}
		if (Niku_x1 <= 0 || Niku_hit == 0) {

			Frame_Count.Niku++;

			if (Beer_x1 <= 0 && Rand_Flag.Niku == 0) {
				Beer_Speed = GetRand(ITEM_SPEED) + 4;
				Frame_Count.Niku_Rand = GetRand(4);
				Rand_Flag.Niku = 1;
			}

			if (Frame_Count.Niku == Frame_Count.Niku_Rand * 60 || Frame_Count.Niku_Rand == 0) {

				Niku_x1 = 620;
				Niku_x2 = Niku_x1 + NUM;
				Niku_y1 = 280 + (GetRand(19) + 1) * 10;
				Niku_y2 = Niku_y1 + NUM;

				Niku_hit = 1;
				Rand_Flag.Niku = 0;
				Frame_Count.Niku = 0;
			}
		}


		// ゲームオーバー
		if ((((Sake_x1 > Chara_x1 && Sake_x1 < Chara_x1 + Chara_w) ||
			(Chara_x1 > Sake_x1 && Chara_x1 < Sake_x1 + Sake_w)) &&
			((Sake_y1 > Chara_y1 && Sake_y1 < Chara_y1 + Chara_h) ||
				(Chara_y1 > Sake_y1 && Chara_y1 < Sake_y1 + Sake_h))
			||
			((Beer_x1 > Chara_x1 && Beer_x1 < Chara_x1 + Chara_w) ||
				(Chara_x1 > Beer_x1 && Chara_x1 < Beer_x1 + Beer_w)) &&
			((Beer_y1 > Chara_y1 && Beer_y1 < Chara_y1 + Chara_h) ||
				(Chara_y1 > Beer_y1 && Chara_y1 < Beer_y1 + Beer_h))
			) &&
			End_flag == 0) {

			End_flag = 1;

			PlaySoundMem(Sound_Effect_Down, DX_PLAYTYPE_BACK);

			mattyo_state = 0;
			mattyo_Speed = -1;
		}

		if (End_flag == 1) {
			Frame_Count.Wait++;
			Dbell_hit = 0;
			Beer_hit = 0;
			Sake_hit = 0;
			Niku_hit = 0;
		}

		if (Frame_Count.Wait == 60 * 2) {
			Frame_Count.Wait = 0;
			End_flag = 0;
			SceneMgr_ChangeScene(eScene_GameOver);
		}


		if (KeyState_Game[KEY_INPUT_ESCAPE] == 1) {          // ポーズ画面

			while (1) {

				GetHitKeyStateAllEx(KeyState_Game);

				DrawGraph(Sky1, 0, Screen_sky, FALSE);
				DrawGraph(Sky2, 0, Screen_sky, FALSE);
				DrawGraph(Sky3, 0, Screen_sky, FALSE);

				DrawGraph(Road1, 260, Screen_road, FALSE);
				DrawGraph(Road2, 260, Screen_road, FALSE);
				DrawGraph(Road3, 260, Screen_road, FALSE);

				DrawExtendGraph(Chara_x1, Chara_y1, Chara_x2, Chara_y2, Chara_Max, TRUE);

				if (End_flag == 1) {

				}

				if (Dbell_hit == 1) {
					DrawExtendGraph(Dbell_x1, Dbell_y1, Dbel_x2, Dbell_y2, Dbell, TRUE);
				}
				if (Beer_hit == 1) {
					DrawExtendGraph(Beer_x1, Beer_y1, Beer_x2, Beer_y2, Beer, TRUE);
				}
				if (Sake_hit == 1) {
					DrawExtendGraph(Sake_x1, Sake_y1, Sake_x2, Sake_y2, Sake, TRUE);
				}
				if (Niku_hit == 1) {
					DrawExtendGraph(Niku_x1, Niku_y1, Niku_x2, Niku_y2, Niku, TRUE);
				}

				DrawString(240, 230, "ゲーム開始(Escキー)", GetColor(0, 0, 0));
				DrawString(240, 260, "ゲーム終了(Eキー)", GetColor(0, 0, 0));

				ScreenFlip();

				ClearDrawScreen();
				if (KeyState_Game[KEY_INPUT_ESCAPE] == 1) {    // ゲーム再開（再開までカウント）
					for (Frame_Count.Pause = 0; Frame_Count.Pause <= 60; Frame_Count.Pause++) {
						DrawGraph(Sky1, 0, Screen_sky, FALSE);
						DrawGraph(Sky2, 0, Screen_sky, FALSE);
						DrawGraph(Sky3, 0, Screen_sky, FALSE);

						DrawGraph(Road1, 260, Screen_road, FALSE);
						DrawGraph(Road2, 260, Screen_road, FALSE);
						DrawGraph(Road3, 260, Screen_road, FALSE);

						DrawExtendGraph(Chara_x1, Chara_y1, Chara_x2, Chara_y2, Chara_Max, TRUE);

						if (Dbell_hit == 1) {
							DrawExtendGraph(Dbell_x1, Dbell_y1, Dbel_x2, Dbell_y2, Dbell, TRUE);
						}
						if (Beer_hit == 1) {
							DrawExtendGraph(Beer_x1, Beer_y1, Beer_x2, Beer_y2, Beer, TRUE);
						}
						if (Sake_hit == 1) {
							DrawExtendGraph(Sake_x1, Sake_y1, Sake_x2, Sake_y2, Sake, TRUE);
						}
						if (Niku_hit == 1) {
							DrawExtendGraph(Niku_x1, Niku_y1, Niku_x2, Niku_y2, Niku, TRUE);
						}
						DrawStringToHandle(290, 260, "●", GetColor(255, 255, 255), Fontsize);
						DrawStringToHandle(290, 260, "③", GetColor(0, 0, 0), Fontsize);

						ScreenFlip();

						ClearDrawScreen();
					}
					for (Frame_Count.Pause = 0; Frame_Count.Pause <= 60; Frame_Count.Pause++) {
						DrawGraph(Sky1, 0, Screen_sky, FALSE);
						DrawGraph(Sky2, 0, Screen_sky, FALSE);
						DrawGraph(Sky3, 0, Screen_sky, FALSE);

						DrawGraph(Road1, 260, Screen_road, FALSE);
						DrawGraph(Road2, 260, Screen_road, FALSE);
						DrawGraph(Road3, 260, Screen_road, FALSE);

						DrawExtendGraph(Chara_x1, Chara_y1, Chara_x2, Chara_y2, Chara_Max, TRUE);

						if (Dbell_hit == 1) {
							DrawExtendGraph(Dbell_x1, Dbell_y1, Dbel_x2, Dbell_y2, Dbell, TRUE);
						}
						if (Beer_hit == 1) {
							DrawExtendGraph(Beer_x1, Beer_y1, Beer_x2, Beer_y2, Beer, TRUE);
						}
						if (Sake_hit == 1) {
							DrawExtendGraph(Sake_x1, Sake_y1, Sake_x2, Sake_y2, Sake, TRUE);
						}
						if (Niku_hit == 1) {
							DrawExtendGraph(Niku_x1, Niku_y1, Niku_x2, Niku_y2, Niku, TRUE);
						}
						DrawStringToHandle(290, 260, "●", GetColor(255, 255, 255), Fontsize);
						DrawStringToHandle(290, 260, "②", GetColor(0, 0, 0), Fontsize);

						ScreenFlip();

						ClearDrawScreen();
					}
					for (Frame_Count.Pause = 0; Frame_Count.Pause <= 60; Frame_Count.Pause++) {
						DrawGraph(Sky1, 0, Screen_sky, FALSE);
						DrawGraph(Sky2, 0, Screen_sky, FALSE);
						DrawGraph(Sky3, 0, Screen_sky, FALSE);

						DrawGraph(Road1, 260, Screen_road, FALSE);
						DrawGraph(Road2, 260, Screen_road, FALSE);
						DrawGraph(Road3, 260, Screen_road, FALSE);

						DrawExtendGraph(Chara_x1, Chara_y1, Chara_x2, Chara_y2, Chara_Max, TRUE);

						if (Dbell_hit == 1) {
							DrawExtendGraph(Dbell_x1, Dbell_y1, Dbel_x2, Dbell_y2, Dbell, TRUE);
						}
						if (Beer_hit == 1) {
							DrawExtendGraph(Beer_x1, Beer_y1, Beer_x2, Beer_y2, Beer, TRUE);
						}
						if (Sake_hit == 1) {
							DrawExtendGraph(Sake_x1, Sake_y1, Sake_x2, Sake_y2, Sake, TRUE);
						}
						if (Niku_hit == 1) {
							DrawExtendGraph(Niku_x1, Niku_y1, Niku_x2, Niku_y2, Niku, TRUE);
						}
						DrawStringToHandle(290, 260, "●", GetColor(255, 255, 255), Fontsize);
						DrawStringToHandle(290, 260, "①", GetColor(0, 0, 0), Fontsize);

						ScreenFlip();
					}
					break;
				}
				if (KeyState_Game[KEY_INPUT_E] == 1) {    // ゲーム終了（メニューに戻る）
					SceneMgr_ChangeScene(eScene_Menu);
					break;
				}
			}

		}
	}
}

void Game_endless_Draw() {

	// 背景
	DrawGraph(Sky1, 0, Screen_sky, FALSE);
	DrawGraph(Sky2, 0, Screen_sky, FALSE);
	DrawGraph(Sky3, 0, Screen_sky, FALSE);

	DrawGraph(Road1, 260, Screen_road, FALSE);
	DrawGraph(Road2, 260, Screen_road, FALSE);
	DrawGraph(Road3, 260, Screen_road, FALSE);

	// スタート
	 if(Start_Draw == 0) {
		DrawStringToHandle(294, 170, "3", GetColor(255, 255, 255), Fontsize);
		DrawStringToHandle(290, 170, "3", GetColor(0, 0, 0), Fontsize);
	}
	if (Start_Draw ==1) {
		DrawStringToHandle(294, 170, "2", GetColor(255, 255, 255), Fontsize);
		DrawStringToHandle(290, 170, "2", GetColor(0, 0, 0), Fontsize);
	}
	if (Start_Draw == 2) {
		DrawStringToHandle(294, 170, "1", GetColor(255, 255, 255), Fontsize);
		DrawStringToHandle(290, 170, "1", GetColor(0, 0, 0), Fontsize);
	}
	if (Start_Draw == 3) {
		DrawStringToHandle(294, 170, "GO!", GetColor(255, 255, 255), Fontsize);
		DrawStringToHandle(290, 170, "GO!", GetColor(0, 0, 0), Fontsize);
	}

	// スコア表示
	DrawFormatString(20, 20, GetColor(0, 0, 0), "Score:%d", Score);


	// デバッグ用(消してもいい)
	if (debug == false) {
		DrawFormatString(20, 40, GetColor(0, 0, 0), "Dbell:%d", Frame_Count.Dbell);
		DrawFormatString(20, 60, GetColor(0, 0, 0), "Beer:%d", Frame_Count.Beer);
		DrawFormatString(20, 80, GetColor(0, 0, 0), "Sake:%d", Frame_Count.Sake);
		DrawFormatString(20, 100, GetColor(0, 0, 0), "Niku:%d", Frame_Count.Niku);
		DrawFormatString(20, 120, GetColor(0, 0, 0), "次の出現時間D:%d", Frame_Count.Dbell_Rand);
		DrawFormatString(20, 140, GetColor(0, 0, 0), "次の出現時間B:%d", Frame_Count.Beer_Rand);
		DrawFormatString(20, 160, GetColor(0, 0, 0), "次の出現時間S:%d", Frame_Count.Sake_Rand);
		DrawFormatString(20, 180, GetColor(0, 0, 0), "次の出現時間N:%d", Frame_Count.Niku_Rand);
		DrawFormatString(20, 200, GetColor(0, 0, 0), "スタート:%d", Start);
	}

	// キャラ表示
	if (mattyo_state == 0) {
		DrawExtendGraph(Chara_x1, Chara_y1, Chara_x2, Chara_y2, Chara_Min, TRUE);
	}
	if (mattyo_state == 2) {
		DrawExtendGraph(Chara_x1, Chara_y1, Chara_x2, Chara_y2, Chara_Max, TRUE);
	}

	if (End_flag == 1) {
		DrawStringToHandle(275, 170, "終了！", GetColor(255, 255, 255), Fontsize);
		DrawStringToHandle(271, 170, "終了！", GetColor(0, 0, 0), Fontsize);
	}

	// アイテム表示(_hitが1の時のみ表示する)
	if (Dbell_hit == 1) {
		DrawExtendGraph(Dbell_x1, Dbell_y1, Dbel_x2, Dbell_y2, Dbell, TRUE);
	}
	if (Beer_hit == 1) {
		DrawExtendGraph(Beer_x1, Beer_y1, Beer_x2, Beer_y2, Beer, TRUE);
	}
	if (Sake_hit == 1) {
		DrawExtendGraph(Sake_x1, Sake_y1, Sake_x2, Sake_y2, Sake, TRUE);
	}
	if (Niku_hit == 1) {
		DrawExtendGraph(Niku_x1, Niku_y1, Niku_x2, Niku_y2, Niku, TRUE);
	}
}
