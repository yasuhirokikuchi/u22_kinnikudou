#pragma once

typedef enum {
    eScene_Menu,    //メニュー画面
    eScene_Tutorial,//チュートリアル画面
    eScene_Game,
    eScene_Game_endless,    //ゲーム画面
    eScene_GameClear,
    eScene_GameOver,

    eScene_None,    //無し
} eScene;

//初期化
void SceneMgr_Initialize();

//終了処理
void SceneMgr_Finalize();

//更新
void SceneMgr_Update();

//描画
void SceneMgr_Draw();

// 引数 nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene nextScene);